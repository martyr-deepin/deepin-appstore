export class Channel {
  /**
   * Execute dbus object methods exposed in window.dstore.channel.objects.xxxx
   * @param {string} method Object name and method name, separated with dot(.)
   * @param args Argument list passed to method.
   */
  static exec(method: string, ...args: any[]) {
    if (window['dstore'] && window['dstore']['channel']) {
      const channel = window['dstore']['channel'];
      const [objectName, methodName] = method.split('.');
      channel['objects'][objectName][methodName](...args);
    }
  }
}
