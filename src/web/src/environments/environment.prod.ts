export const environment = {
  production: true,

  operationList: {
    CN: 'http://10.0.10.70:18100',
    Default: 'http://10.0.10.70:18100',
  } as { [key: string]: string },
  region: 'Default',
  autoSelect: true,
  operationServer: '',
  metadataServer: 'https://dstore-metadata.deepin.cn',
  themeName: 'light',
  locale: 'zh_CN',
  native: false,
  supportSignIn: false,
};
