package version

import (
	"reflect"
	"testing"
)

func TestVersion(t *testing.T) {
	cases := []struct {
		v1       string
		v2       string
		expected bool // If true, v1 less than v2
	}{
		// RedHat
		{"7.4.629-3", "7.4.629-5", true},
		{"7.4.622-1", "7.4.629-1", true},
		{"6.0-4.el6.x86_64", "6.0-5.el6.x86_64", true},
		{"6.0-4.el6.x86_64", "6.1-3.el6.x86_64", true},
		{"7.0-4.el6.x86_64", "6.1-3.el6.x86_64", false},
		// Debian
		{"2:7.4.052-1ubuntu3", "2:7.4.052-1ubuntu3.1", true},
		{"2:7.4.052-1ubuntu2", "2:7.4.052-1ubuntu3", true},
		{"2:7.4.052-1", "2:7.4.052-1ubuntu3", true},
		{"2:7.4.052", "2:7.4.052-1", true},
		{"1:7.4.052", "2:7.4.052", true},
		{"1:7.4.052", "7.4.052", false},
		{"2:7.4.052-1ubuntu3.2", "2:7.4.052-1ubuntu3.1", false},
	}
	for _, tc := range cases {
		v1, _ := NewVersion(tc.v1)
		v2, _ := NewVersion(tc.v2)
		actual := v1.LessThan(v2)
		if actual != tc.expected {
			t.Fatalf("v1: %v\nv2: %v\nexpected: %v\nactual: %v",
				tc.v1, tc.v2, tc.expected, actual)
		}
	}

}

func TestNewVersion(t *testing.T) {
	cases := []struct {
		version  string
		expected Version
		err      bool
	}{
		{"1.2.3", Version{0, "1.2.3", ""}, false},
		{"1:1.2.3", Version{1, "1.2.3", ""}, false},
		{"A:1.2.3", Version{}, true},
		{"-1:1.2.3", Version{}, true},
		{"6.0-4.el6.x86_64", Version{0, "6.0", "4.el6.x86_64"}, false},
		{"6.0-9ubuntu1.5", Version{0, "6.0", "9ubuntu1.5"}, false},
		{"2:7.4.052-1ubuntu3.1", Version{2, "7.4.052", "1ubuntu3.1"}, false},
		{"2:-1ubuntu3.1", Version{}, true},
		{"2:A7.4.052-1ubuntu3.1", Version{}, true},
		{"2:7.4.!052-1ubuntu3.1", Version{}, true},
		{"7.4.052-!1ubuntu3.1", Version{}, true},
	}

	for _, tc := range cases {
		actual, err := NewVersion(tc.version)
		if tc.err && err == nil {
			t.Fatalf("expected error for version: %s", tc.version)
		} else if !tc.err && err != nil {
			t.Fatalf("unexpected error for version %s: %v", tc.version, err)
		} else if !tc.err && err == nil {
			if actual.epoch != tc.expected.epoch {
				t.Fatalf(
					"version: %s\nexpected: %v\nactual: %v",
					tc.version, tc.expected, actual)
			}
		}
	}
}

func TestEqual(t *testing.T) {
	cases := []struct {
		v1       Version
		v2       Version
		expected bool // If true, v1 = v2
	}{
		{Version{2, "7.4.052", "1ubuntu3"}, Version{2, "7.4.052", "1ubuntu3.1"}, false},
		{Version{2, "7.4.052", "1ubuntu2"}, Version{2, "7.4.052", "1ubuntu3"}, false},
		{Version{2, "7.4.052", "1ubuntu3"}, Version{2, "7.4.052", "1ubuntu3"}, true},
		{Version{2, "7.4.052", "1ubuntu1"}, Version{2, "7.4.052", "1"}, false},
		{Version{upstreamVersion: "7.4.052"}, Version{upstreamVersion: "7.4.052"}, true},
	}
	for _, tc := range cases {
		actual := tc.v1.Equal(tc.v2)
		if actual != tc.expected {
			t.Fatalf("v1: %v\nv2: %v\nexpected: %v\nactual: %v",
				tc.v1, tc.v2, tc.expected, actual)
		}
	}
}

func TestGreaterThan(t *testing.T) {
	cases := []struct {
		v1       Version
		v2       Version
		expected bool // If true, v1 greater than v2
	}{
		{Version{2, "7.4.052", "1ubuntu3"}, Version{2, "7.4.052", "1ubuntu3.1"}, false},
		{Version{2, "7.4.052", "1ubuntu2"}, Version{2, "7.4.052", "1ubuntu3"}, false},
		{Version{2, "7.4.052", "1ubuntu1"}, Version{2, "7.4.052", "1"}, true},
		{Version{2, "6.0", "9ubuntu1.4"}, Version{2, "6.0", "9ubuntu1.5"}, false},
		{
			Version{upstreamVersion: "7.4.052"},
			Version{upstreamVersion: "7.4.052", debianRevision: "1ubuntu2"},
			false,
		},
		{
			Version{upstreamVersion: "7.4.052"},
			Version{epoch: 2, upstreamVersion: "7.4.052"},
			false,
		},
	}
	for _, tc := range cases {
		actual := tc.v1.GreaterThan(tc.v2)
		if actual != tc.expected {
			t.Fatalf("v1: %v\nv2: %v\nexpected: %v\nactual: %v",
				tc.v1, tc.v2, tc.expected, actual)
		}
	}
}

func TestLessThan(t *testing.T) {
	cases := []struct {
		v1       Version
		v2       Version
		expected bool // If true, v1 less than v2
	}{
		{Version{2, "7.4.052", "1ubuntu3"}, Version{2, "7.4.052", "1ubuntu3.1"}, true},
		{Version{2, "7.4.052", "1ubuntu2"}, Version{2, "7.4.052", "1ubuntu3"}, true},
		{Version{2, "7.4.052", "1ubuntu1"}, Version{2, "7.4.052", "1"}, false},
		{Version{2, "6.0", "9ubuntu1.4"}, Version{2, "6.0", "9ubuntu1.5"}, true},
		{
			Version{upstreamVersion: "7.4.052"},
			Version{upstreamVersion: "7.4.052", debianRevision: "1ubuntu2"},
			true,
		},
		{
			Version{upstreamVersion: "1.9.1", debianRevision: "2"},
			Version{upstreamVersion: "1.16", debianRevision: "1+deb8u1"},
			true,
		},
		{
			Version{upstreamVersion: "1.9", debianRevision: "2"},
			Version{upstreamVersion: "1.9.1", debianRevision: "1+deb8u1"},
			true,
		},
		{
			Version{upstreamVersion: "7.4.052"},
			Version{epoch: 2, upstreamVersion: "7.4.052"},
			true,
		},
	}
	for _, tc := range cases {
		actual := tc.v1.LessThan(tc.v2)
		if actual != tc.expected {
			t.Fatalf("v1: %v\nv2: %v\nexpected: %v\nactual: %v",
				tc.v1, tc.v2, tc.expected, actual)
		}
	}
}

func TestString(t *testing.T) {
	cases := []struct {
		v        Version
		expected string
	}{
		{Version{2, "7.4.052", "1ubuntu3"}, "2:7.4.052-1ubuntu3"},
		{Version{2, "7.4.052", "1"}, "2:7.4.052-1"},
		{Version{0, "7.4.052", "1"}, "7.4.052-1"},
		{Version{upstreamVersion: "7.4.052", debianRevision: "1"}, "7.4.052-1"},
		{Version{epoch: 1, upstreamVersion: "7.4.052"}, "1:7.4.052"},
		{Version{upstreamVersion: "7.4.052"}, "7.4.052"},
	}
	for _, tc := range cases {
		actual := tc.v.String()
		if actual != tc.expected {
			t.Fatalf("v: %v\n\nexpected: %v\nactual: %v",
				tc.v, tc.expected, actual)
		}
	}
}

func TestCompare(t *testing.T) {
	cases := []struct {
		v1       string
		v2       string
		negative bool // If true, v1 less than v2
	}{
		{"6.4.052", "7.4.052", true},
		{"6.4.052", "6.5.052", true},
		{"6.4.052", "6.4.053", true},
		{"1ubuntu1", "1ubuntu3.1", true},
		{"1", "1ubuntu1", true},
		{"7.4.027", "7.4.052", true},
	}
	for _, tc := range cases {
		ret := compare(tc.v1, tc.v2)
		if tc.negative && ret > 0 {
			t.Fatalf("v1: %s\nv2: %s\nexpected: %v\nactual: %v",
				tc.v1, tc.v2, tc.negative, ret < 0)
		}
	}
}

func TestExtract(t *testing.T) {
	cases := []struct {
		version     string
		expectedNum defaultNumSlice
		expectedStr defaultStringSlice
	}{
		{"1.2.3", []int{1, 2, 3}, defaultStringSlice{".", "."}},
		{"12.+34.~45", []int{12, 34, 45}, []string{".+", ".~"}},
		{".+-:~123.45", []int{123, 45}, []string{".+-:~", "."}},
	}
	for _, tc := range cases {
		n, s := extract(tc.version)
		if !reflect.DeepEqual(n, tc.expectedNum) {
			t.Fatalf("version: %s\nexpected: %v\nactual: %v",
				tc.version, tc.expectedNum, n)
		}
		if !reflect.DeepEqual(s, tc.expectedStr) {
			t.Fatalf("version: %s\nexpected: %v\nactual: %v",
				tc.version, tc.expectedStr, s)
		}
	}
}

func TestOrder(t *testing.T) {
	cases := []struct {
		r        rune
		expected int
	}{
		{'A', 65},
		{'~', -1},
		{'\a', 263},
	}
	for _, tc := range cases {
		o := order(tc.r)
		if o != tc.expected {
			t.Fatalf("rune: %c\n\nexpected: %d\nactual: %d",
				tc.r, tc.expected, o)
		}
	}

}

func TestCompareString(t *testing.T) {
	cases := []struct {
		s1       string
		s2       string
		negative bool // If true, s1 less than s2
	}{
		{"~~", "~~a", true},
		{"~~a", "~", true},
		{"~", "", true},
		{"", "a", true},
		{"a", ".", true},
	}
	for _, tc := range cases {
		ret := compareString(tc.s1, tc.s2)
		if tc.negative && ret > 0 {
			t.Fatalf("s1: %s\ns2: %s\nexpected: %v\nactual: %v",
				tc.s1, tc.s2, tc.negative, ret < 0)
		}
	}
}
