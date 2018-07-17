// Copyright (c) 2015 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef SERVICE_BACKEND_CHINESE_TO_PINYIN_H_
#define SERVICE_BACKEND_CHINESE_TO_PINYIN_H_

#include <QString>

namespace dstore {

/**
 * Convert Chinese word into pinyin, with syllable.
 * @param words
 * @return
 */
QString Chinese2Pinyin(const QString& words);

/**
 * Convert Chinese word into pinyin, without syllable.
 * @param words
 * @return
 */
QString Chinese2PinyinNoSyl(const QString& words);

}  // namespace dstore

#endif  // SERVICE_BACKEND_CHINESE_TO_PINYIN_H_
