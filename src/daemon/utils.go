package main

import (
	"compress/gzip"
	"encoding/json"
	"io"
	"net/http"
	"os"
	"time"
)

type BodyHook func(cacheFilepath string, rc io.ReadCloser) error

func cacheFetchHook(
	url string,
	cacheFilepath string,
	expire time.Duration,
	hook BodyHook) error {

	fi, _ := os.Stat(cacheFilepath)
	if (fi != nil) && (fi.Size() > 0) && (time.Now().Sub(fi.ModTime()) < expire) {
		return nil
	}

	client := http.DefaultClient
	request, err := http.NewRequest("GET", url, nil)
	request.Header.Add("Accept-Encoding", "gzip")
	if fi != nil {
		request.Header.Add("If-Modified-Since", fi.ModTime().Format(time.RFC1123))
	}
	resp, err := client.Do(request)
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	lastModified, _ := time.Parse(time.RFC1123, resp.Header.Get("Last-Modified"))
	if (fi != nil) && lastModified.Sub(fi.ModTime()) <= 0 {
		// update modify time
		now := time.Now()
		os.Chtimes(cacheFilepath, now, now)
		return nil
	}

	var reader io.ReadCloser
	switch resp.Header.Get("Content-Encoding") {
	case "gzip":
		reader, err = gzip.NewReader(resp.Body)
		if err != nil {
			return err
		}
		defer reader.Close()
	default:
		reader = resp.Body
	}

	return hook(cacheFilepath, reader)
}

func cacheFetch(url, cacheFilepath string, expire time.Duration) error {
	hook := func(cacheFilepath string, reader io.ReadCloser) error {
		f, err := os.OpenFile(cacheFilepath, os.O_WRONLY|os.O_CREATE, 0644)
		defer f.Close()
		if err != nil {
			return err
		}

		_, err = io.Copy(f, reader)
		return err
	}
	return cacheFetchHook(url, cacheFilepath, expire, hook)
}

func cacheFetchJSON(v interface{}, url, cacheFilepath string, expire time.Duration) error {
	cacheFetch(url, cacheFilepath, expire)

	f, err := os.Open(cacheFilepath)
	if err != nil {
		return err
	}
	defer f.Close()

	jsonDec := json.NewDecoder(f)

	return jsonDec.Decode(v)
}
