// Copyright (c) 2015 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "services/backend/chinese2pinyin.h"

#include <QHash>
#include <QTextStream>

#include "base/file_util.h"

namespace dstore {

namespace {

static QHash<uint32_t, QString> dict = { };

const char kDictFile[] = ":/services/backend/pinyin.dict";

void InitDict() {
  if (dict.size() != 0) {
    return;
  }

  QString content = ReadFile(kDictFile);
  QTextStream stream(&content, QIODevice::ReadOnly);
  while (!stream.atEnd()) {
    const QString line = stream.readLine();
    const QStringList items = line.split(QChar(':'));
    if (items.size() == 2) {
      dict.insert(static_cast<uint32_t>(items[0].toInt(nullptr, 16)), items[1]);
    }
  }
}

}  // namespace

QString Chinese2Pinyin(const QString& words) {
  InitDict();

  QString result;
  for (int i = 0; i < words.length(); ++i) {
    const uint32_t key = static_cast<uint32_t>(words.at(i).unicode());
    auto find_result = dict.find(key);
    if (find_result != dict.end()) {
      result.append(find_result.value());
    } else {
      result.append(words.at(i));
    }
  }
  return result;
}

}  // namespace dstore
