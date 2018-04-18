declare const require;

export class Locale {
  /**
   * Browser language like 'en-US', 'zh-CN'.
   * @returns {string}
   */
  static getBrowserLocale(): string {
    return navigator.language;
  }

  /**
   * Unix like locale name, e.g. 'zh_CN', 'en_US'.
   * @returns {string}
   */
  static getUnixLocale(): string {
    const lang = navigator.language;
    const langMap = {
      'en-GB': 'en_GB',
      'es-419': 'es_419',
      'pt-BR': 'pt_BR',
      'pt-PT': 'pt_PT',
      'zh-CN': 'zh_CN',
      'zh-TW': 'zh_TW',
    };
    if (lang in langMap) {
      return langMap[lang];
    }
    return lang;
  }

  /**
   * Angular uses PCP47 locale identifies.
   * @returns {string}
   */
  static getPcp47Locale(): string {
    const lang = navigator.language;

    // See https://angular.io/guide/i18n#setting-up-the-locale-of-your-app
    const langMap = {
      'zh-CN': 'zh-Hans',
      'zh-HK': 'zh-Hant-HK',
      'zh-TW': 'zh-Hant',
    };
    if (lang in langMap) {
      return langMap[lang];
    } else {
      return lang;
    }
  }

  static getUnixLocaleFilePath(): string {
    const locale = Locale.getUnixLocale();
    // Note that xlf files are placed in ../../locale folder.
    return `../../locale/messages.${locale}.xlf`;
  }

  static getUnixLocaleFileContent(): string {
    try {
      const locale = Locale.getUnixLocale();
      // Note that xlf files are placed in ../../locale folder.
      return require(`raw-loader!../../locale/messages.${locale}.xlf`);
    } catch (error) {
      return '';
    }
  }

  static localeFileExists(): boolean {
    try {
      const locale = Locale.getUnixLocale();
      // Note that xlf files are placed in ../../locale folder.
      require(`raw-loader!../../locale/messages.${locale}.xlf`);
      return true;
    } catch (error) {
      return false;
    }
  }
}
