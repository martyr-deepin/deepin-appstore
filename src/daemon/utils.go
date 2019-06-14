package main

import (
	"compress/gzip"
	"encoding/json"
	"io"
	"net/http"
	"os"
	"time"
)

var cacheFolder string
var configFolder string
var iconFolder string

func cacheFetch(url, cacheFilepath string, expire time.Duration) error {
	fi, _ := os.Stat(cacheFilepath)
	if (fi != nil) && (fi.Size() > 0) && (time.Now().Sub(fi.ModTime()) < expire) {
		return nil
	}

	client := http.DefaultClient
	request, err := http.NewRequest("GET", url, nil)
	request.Header.Add("Accept-Encoding", "gzip")
	resp, err := client.Do(request)
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	gzipReader, err := gzip.NewReader(resp.Body)
	if err != nil {
		return err
	}

	f, err := os.OpenFile(cacheFilepath, os.O_WRONLY|os.O_CREATE, 0644)
	defer f.Close()
	if err != nil {
		return err
	}
	_, err = io.Copy(f, gzipReader)
	return err
}

// Check file in cache
func cacheFetchJSON(v interface{}, url, cacheFilepath string, expire time.Duration) error {
	fi, _ := os.Stat(cacheFilepath)
	if (fi != nil) && (time.Now().Sub(fi.ModTime()) < expire) {
		f, err := os.Open(cacheFilepath)
		if err != nil {
			logger.Error("open cache file %v failed: %v", cacheFilepath, err)
			return err
		}

		jsonDec := json.NewDecoder(f)
		return jsonDec.Decode(v)
	}

	client := http.DefaultClient
	request, err := http.NewRequest("GET", url, nil)
	request.Header.Add("Accept-Encoding", "gzip")
	resp, err := client.Do(request)
	if err != nil {
		logger.Error("GET %v failed: %v", url, err)
		return err
	}
	defer resp.Body.Close()

	var reader io.ReadCloser
	switch resp.Header.Get("Content-Encoding") {
	case "gzip":
		reader, err = gzip.NewReader(resp.Body)
		if err != nil {
			logger.Errorf("gzip data read %q failed: %v", url, err)
			return err
		}
		defer reader.Close()
	default:
		reader = resp.Body
	}

	jsonDec := json.NewDecoder(reader)
	err = jsonDec.Decode(v)
	if err != nil {
		logger.Error("json decode failed: %v", err)
		return err
	}

	data, err := json.Marshal(v)
	if err != nil {
		logger.Error("json marshal failed: %v", err)
		return err
	}

	f, err := os.OpenFile(cacheFilepath, os.O_WRONLY|os.O_CREATE, 0644)
	if err != nil {
		logger.Error("create cache %v failed: %v", cacheFilepath, err)
		return err
	}
	_, err = f.Write(data)
	if err != nil {
		logger.Error("write cache %v failed: %v", cacheFilepath, err)
		return err
	}
	return err
}
