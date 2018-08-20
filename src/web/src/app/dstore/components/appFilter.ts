export type AppFilterFunc = (appName: string) => boolean;
export function Allowed(appName: string) {
  return true;
}
