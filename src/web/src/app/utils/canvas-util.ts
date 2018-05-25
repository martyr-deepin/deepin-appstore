export class CanvasUtil {
  /**
   * Convert the content of an img element into base64-encoded string.
   * @param {HTMLImageElement} img
   * @param {string} format Output image format, default is JPEG.
   * @returns {string}
   */
  static getBase64Image(img: HTMLImageElement, format = 'image/jpg'): string {
    const canvas = document.createElement('canvas');
    canvas.width = img.naturalWidth;
    canvas.height = img.naturalHeight;

    const ctx = canvas.getContext('2d');
    ctx.drawImage(img, 0, 0);

    const dataURL = canvas.toDataURL(format);
    return dataURL.replace(/^data:image\/(png|jpg);base64,/, '');
  }
}
