export const environment = {
  production: true,

  operationList: {
    CN: 'https://dstore-operation-china.deepin.cn/',
    Default: 'https://dstore-operation-international.deepin.cn',
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
