declare const require;

export class Locale {

  static getAngularLocale(): string {
    const lang = navigator.language;

    // See https://angular.io/guide/i18n#setting-up-the-locale-of-your-app
    const langMap = {
      'zh-CN': 'zh-Hans',
      'zh-HK': 'zh-Hant-HK',
      'zh-TW': 'zh-Hant'
    };
    if (lang in langMap) {
      return langMap[lang];
    } else {
      return lang;
    }
  }

  static localeFileExists(locale: string): boolean {
    try {
      // Note that xlf files are placed in ../../locale folder.
      const translations = require(`raw-loader!../../locale/messages.${locale}.xlf`);
      return true;
    } catch (error) {
      return false;
    }
  }
}
