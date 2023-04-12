import { RcFile, UploadFile } from 'antd/es/upload'

export const scrollToTop = () => {
  window?.scrollTo({
    behavior: 'smooth',
    top: 0
  })
}

export const validateFileType = (file: UploadFile, allowedTypes?: string) => {
  if (!allowedTypes) {
    return true
  }

  if (file.type) {
    return allowedTypes.includes(file.type)
  }
}

export const fileToBlob = async (file: RcFile): Promise<Blob> =>
  new Blob([new Uint8Array(await file.arrayBuffer())], { type: file.type })

export const fileToBase64 = (file: RcFile | Blob, callback: (url: string) => void) => {
  const reader = new FileReader()
  reader.onload = function () {
    callback(reader.result as string)
  }
  reader.onerror = function () {
    callback('')
  }
  reader.readAsDataURL(file)
}
