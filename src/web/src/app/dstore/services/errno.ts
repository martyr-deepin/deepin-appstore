export interface Error {
  code: number;
  content: string;
  key: string;
}

export enum ErrorCode {
  CodeInternalServerError = 1,
  CodeIsExisted,
  CodeNotFound,
  CodeApiRateLimit,
  CodeBadParams,
  CodeAuthorizedFailed,
  CodeAuthorizedOutDate,
  CodeRuleFault,
  CodeForceSync
}
export const ErrorCodeString = [
  '内部错误',
  '已存在',
  '未找到',
  '过于频繁',
  '参数错误',
  '认证失败',
  '认证超时',
  '权限不足',
  '强制刷新'
];

export enum ImageError {
  Unknown,
  File,
  Format,
  ImgSize,
  FileSize
}
export const ImageErrorString = [
  '未知错误',
  '非图片文件',
  '图片格式错误',
  '图片尺寸错误',
  '图片文件大小超出限制'
];
