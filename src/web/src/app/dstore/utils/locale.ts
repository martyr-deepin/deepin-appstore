export class Locale {
  /**
   * Unix like locale name, e.g. 'zh_CN', 'en_US'.
   * @returns {string}
   */
  static getUnixLocale(): string {
    const lang = navigator.language;
    const langList = [
      'en-GB',
      'en-US',
      'es-419',
      'pt-BR',
      'pt-PT',
      'zh-CN',
      'zh-TW'
    ];
    if (langList.includes(lang)) {
      return lang.replace('-', '_');
    }
    return lang;
  }
}
