import { Route } from 'src/interface/app'

// contanst url
const PATH_URL = {
  home: '/',
  admin: '/admin',
  login: '/admin/login',
  events: '/admin/overview',
  sponsors: '/admin/garbage',
  notifications: '/admin/notifications',
  memberDetail: ':memberID'
} as const

// private routes (path, component)
export const PRIVATE_ROUTE: Route[] = [
  {
    path: PATH_URL.sponsors,
    element: () => import('src/pages/Admin/sponsor/Sponsors')
  },

  {
    path: PATH_URL.events,
    element: () => import('src/pages/Admin/Events')
  },
  {
    path: PATH_URL.notifications,
    element: () => import('src/pages/Notify')
  }
]

export default PATH_URL
