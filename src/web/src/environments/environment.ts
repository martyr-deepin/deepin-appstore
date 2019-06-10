// The file contents for the current environment will overwrite these during build.
// The build system defaults to the dev environment which uses `environment.ts`, but if you do
// `ng build --env=prod` then `environment.prod.ts` will be used instead.
// The list of which env maps to which file can be found in `.angular-cli.json`.

export const environment = {
  production: false,

  // Default server address only used in browser mode.
  metadataServer: 'http://10.0.10.70:18000',
  operationServer: 'http://10.0.10.70:18100',
  region: 0, // 0:China 1:International
  themeName: 'light',
  supportSignIn: true,
  locale: 'zh_CN',
};
