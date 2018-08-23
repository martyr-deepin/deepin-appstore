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
      try {
        channel['objects'][objectName][methodName](...args);
      } catch (error) {
        console.error(method, args, error);
      }
    }
  }

  static registerCallback(method: string, callback: (...args: any[]) => void): void {
    // console.log('registerCallback', method);
    if (window['dstore'] && window['dstore']['channel']) {
      const channel = window['dstore']['channel'];
      const [objectName, signalName] = method.split('.');
      try {
        channel['objects'][objectName][signalName].connect(callback);
      } catch (error) {
        console.error(method, error);
      }
    }
  }

  static unregisterCallback(method: string, callback: (...args: any[]) => void): void {
    // console.log('unregisterCallback', method);
    if (window['dstore'] && window['dstore']['channel']) {
      const channel = window['dstore']['channel'];
      const [objectName, signalName] = method.split('.');
      try {
        channel['objects'][objectName][signalName].disconnect(callback);
      } catch (error) {
        console.error(method, error);
      }
    }
  }

  /**
   * Execute dbus object methods and receive its return value
   * @param {(resp: any) => void} callback
   * @param {string} method
   * @param args
   */
  static execWithCallback(
    callback: (resp: any) => void,
    method: string,
    ...args: any[]
  ): () => void {
    // console.log('execWithCallback(): ', method, ', args: ', ...args);
    if (window['dstore'] && window['dstore']['channel']) {
      const channel = window['dstore']['channel'];
      const [objectName, methodName] = method.split('.');
      try {
        const signalName = method + 'Reply';
        console.log('method', method, 'signal', signalName);
        Channel.registerCallback(signalName, callback);
        channel['objects'][objectName][methodName](...args);
        return Channel.unregisterCallback.bind(Channel, signalName, callback);
      } catch (error) {
        console.error(callback, 'method: ', method, ', args:', args, ', error: ', error);
      }
    }
  }
}
