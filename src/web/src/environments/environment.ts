// The file contents for the current environment will overwrite these during build.
// The build system defaults to the dev environment which uses `environment.ts`, but if you do
// `ng build --env=prod` then `environment.prod.ts` will be used instead.
// The list of which env maps to which file can be found in `.angular-cli.json`.

export const environment = {
  production: false,

  // Default server address only used in browser mode.

  operationList: {
    CN: 'http://10.0.10.70:18100',
    Default: 'http://10.0.10.67:18100',
  } as { [key: string]: string },
  region: 'Default', // 0:China 1:International
  autoSelect: true, // auto select operation by ip
  operationServer: '', // operation server

  metadataServer: 'http://10.0.10.70:18000', //metadata server
  themeName: 'light', // theme
  locale: 'zh_CN', // language
  native: false, // native or browser
  supportSignIn: true, // support sign in
};
