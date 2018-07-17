#!/usr/bin/env python3
# Copyright (c) 2018 Xu Shaohua <xushaohua2016@outlook.com>.
# All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Update message xlf files.
# * Add missing <target> tags
# * Unescape invalid characters in text of <target> tags.

# Required packages:
# * python3
# * python3-lxml

import html
import os

from lxml import etree


def fix_xlf(xlf_file):
    TRANS_TAG = "//{urn:oasis:names:tc:xliff:document:1.2}trans-unit"
    SOURCE_TAG = "{urn:oasis:names:tc:xliff:document:1.2}source"
    TARGET_TAG = "{urn:oasis:names:tc:xliff:document:1.2}target"
    print("fix xlf:", xlf_file)
    root = etree.parse(xlf_file)
    trans_elems = root.findall(TRANS_TAG)
    for trans_elem in trans_elems:
        # Add missing <target> tag
        children = trans_elem.getchildren()
        find_target = False
        source_value = ""
        source_idx = -1
        for index, child in enumerate(children):
            if child.tag == TARGET_TAG:
                find_target = True
            elif child.tag == SOURCE_TAG:
                source_value = child.text
                source_idx = index
        if not find_target:
            target_elem = etree.Element("target")
            trans_elem.insert(source_idx + 1, target_elem)
            target_elem.text = source_value

        # Escape values in <target>
#        for child in trans_elem.getchildren():
#            if child.tag == TARGET_TAG:
#                if child.text:
#                    text = child.text.replace("&lt;", "<")
#                    text = text.replace("&gt;", ">")
#                    text = text.replace("&quot;", '"')
#                    child.text = text
#                break

        # Write back
        with open(xlf_file, "wb") as fh:
            # First, write xml head
            fh.write(b'<?xml version="1.0" ?>\n')
            content = etree.tostring(root, pretty_print=True, encoding='utf8')
            # Escape HTML special chars in <target>
            content = content.replace(b"&lt;", b"<")
            content = content.replace(b"&gt;", b">")
            content = content.replace(b"&quot;", b'"')
            fh.write(content)


def main():
    for filename in os.listdir("src/locale"):
        if filename == "messages.xlf":
            continue
        if not filename.endswith(".xlf"):
            continue
        xlf_file = os.path.join("src", "locale", filename)
        fix_xlf(xlf_file)


if __name__ == "__main__":
    main()
