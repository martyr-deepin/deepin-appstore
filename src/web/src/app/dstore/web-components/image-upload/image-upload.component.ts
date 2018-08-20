import {
  Component,
  OnInit,
  ViewChild,
  ElementRef,
  Output,
  Input,
  EventEmitter,
  HostListener,
} from '@angular/core';
import { HttpClient, HttpParams } from '@angular/common/http';
import { DomSanitizer, SafeUrl } from '@angular/platform-browser';
import { from, fromEvent, merge } from 'rxjs';
import { map, flatMap, filter, catchError } from 'rxjs/operators';
import * as localForage from 'localforage';

import { ImageError, ImageErrorString } from '../../services/errno';
import { BaseService } from '../../services/base.service';
import { SizeHuman } from '../../pipes/size-human';
import { ImageType } from '../../services/app';

@Component({
  selector: 'app-image-upload',
  templateUrl: './image-upload.component.html',
  styleUrls: ['./image-upload.component.scss'],
})
export class ImageUploadComponent implements OnInit {
  constructor(
    private el: ElementRef<HTMLDivElement>,
    private domSanitizer: DomSanitizer,
    private http: HttpClient,
  ) {}
  @ViewChild('imageInput') imageInput: ElementRef<HTMLInputElement>;
  @Input()
  set src(src: string) {
    if (src) {
      this.store.getItem<string>(src).then(data => {
        if (data) {
          this.imgSrc = this.domSanitizer.bypassSecurityTrustUrl(data);
        } else {
          this.imgSrc = this.server + '/' + src;
        }
      });
    }
  }
  @Input() server: string;
  @Input() type: ImageType;
  @Input() title: string;
  @Input() width: number;
  @Input() height: number;
  @Input() size: number;
  @Input() formats: string[];
  @Input() multiple = false;
  @Output() upload = new EventEmitter<string>(true);
  @Output() error = new EventEmitter<ImageError>(true);

  store = localForage.createInstance({ name: 'images' });
  imgSrc: SafeUrl;

  ngOnInit() {
    if (!sessionStorage.getItem('imagesClear')) {
      this.store.clear();
      sessionStorage.setItem('imagesClear', 'imagesClear');
    }
    this.error.subscribe(err => console.error(ImageErrorString[err]));

    merge(
      ...['dragleave', 'drop', 'dragenter', 'dragover'].map(eName => fromEvent(document, eName)),
    ).subscribe(e => {
      e.preventDefault();
    });
    fromEvent(this.el.nativeElement, 'drop').subscribe((e: DragEvent) => {
      console.log(e.dataTransfer.files);
    });

    merge(
      fromEvent(this.imageInput.nativeElement, 'change').pipe(
        map(() => this.imageInput.nativeElement.files),
      ),
      fromEvent(this.el.nativeElement, 'drop').pipe(map((e: DragEvent) => e.dataTransfer.files)),
    )
      .pipe(
        // multiple file
        flatMap(files => from(files)),
        // check file
        filter(file => {
          // check file size
          if (this.size && file.size > this.size) {
            this.error.emit(ImageError.FileSize);
            return false;
          }
          // max size 12M
          if (file.size > 12e7) {
            this.error.emit(ImageError.FileSize);
            return false;
          }
          if (file.type) {
            // check file format
            const [type, format] = file.type.split('/');
            if (type !== 'image') {
              this.error.emit(ImageError.File);
              return false;
            }
            if (this.formats && !this.formats.map(t => MimeType[t] || t).includes(format)) {
              this.error.emit(ImageError.Format);
              return false;
            }
          }
          return true;
        }),
        // read image file
        flatMap(file => {
          const r = new FileReader();
          r.onerror = () => this.error.emit(ImageError.Unknown);
          r.readAsDataURL(file);
          return fromEvent(r, 'load').pipe(
            // loading to Image Element
            flatMap(() => {
              const img = new Image();
              img.onerror = () => this.error.emit(ImageError.File);
              img.src = r.result;
              return fromEvent(img, 'load').pipe(map(() => img));
            }),
            // check image width and height
            filter(img => {
              if (this.width && this.height) {
                if (this.width !== img.width || this.height !== img.height) {
                  this.error.emit(ImageError.ImgSize);
                  return false;
                }
              }
              return true;
            }),
            flatMap(img => {
              const formData = new FormData();
              formData.append('file', file);
              formData.append('type', `${this.type || 1}`);

              return this.http.post<{ path: string }>(this.server + '/api/upload', formData).pipe(
                flatMap(result => {
                  // preview cache
                  const canvas = document.createElement('canvas');
                  canvas.width = this.el.nativeElement.clientWidth;
                  canvas.height = (canvas.width * img.height) / img.width;
                  console.log(canvas.width, canvas.height);
                  canvas.getContext('2d').drawImage(img, 0, 0, canvas.width, canvas.height);
                  return from(this.store.setItem(result.path, canvas.toDataURL()));
                }, result => result.path),
              );
            }),
          );
        }),
        catchError((err, caught) => caught),
      )
      .subscribe(path => {
        this.upload.emit(path);
      });
  }
  // 图片类型
  get accept() {
    if (!this.formats) {
      return 'image/*';
    }
    return this.formats.map(t => `image/${MimeType[t] || t}`).join(',');
  }
  // 图片描述
  get desc() {
    const strArr = [];
    if (this.title) {
      strArr.push(this.title);
    }
    if (this.formats) {
      strArr.push('格式' + this.formats.join('，'));
    }
    if (this.width && this.height) {
      strArr.push(`尺寸${this.width}X${this.height}`);
    }
    if (this.size) {
      strArr.push(`最大${new SizeHuman().transform(this.size)}`);
    }
    return strArr.join('，');
  }
}

const MimeType = {
  jpg: 'jpeg',
  svg: 'svg+xml',
};
