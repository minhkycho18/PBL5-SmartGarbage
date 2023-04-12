import { IdcardOutlined, SolutionOutlined, TeamOutlined } from '@ant-design/icons'

export const urlRegex =
  /[(http(s)?):\/\/(www\.)?a-zA-Z0-9@:%._\+~#=]{2,256}\.[a-z]{2,6}\b([-a-zA-Z0-9@:%_\+.~#?&//=]*)/gi

export const SECONDS_IN_DAY = 86400

export const SIZEBAR_OPTIONS = [
  {
    key: 'overview',
    label: 'Overview',
    path: '/admin/overview',
    icon: SolutionOutlined
  },
  {
    key: 'garbage',
    label: 'Garbage',
    path: '/admin/garbage',
    icon: IdcardOutlined
  }
]

export const ERROR_MESSAGE = {
  required: 'This field is required.',
  invalid: 'This field is invalid.',
  invalidImage: 'Uploaded file is not a valid image. Only JPG, PNG files are allowed.'
}

export const TOOLTIP_MESSAGE = {
  image: 'Only JPG, PNG files are allowed.'
}

export const IMAGE_FILETYPE = 'image/png, image/jpeg'
