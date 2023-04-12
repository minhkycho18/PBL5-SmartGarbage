import { Navigate, useRoutes } from 'react-router-dom'
import PATH_URL, { PRIVATE_ROUTE } from './shared/path'

// component
import { Suspense, lazy } from 'react'
import { Route } from './interface/app'
import NotFoundPage from './pages/NotFoundPage'
import PrivateRoute from './routes/PrivateRoutes'
import { AddSponsor } from './pages/Admin/sponsor'

interface RouteElement {
  routeElement: () => Promise<any>
  isPrivate?: boolean
}

interface LazyRouteProps {
  routes: Route[]
  isPrivate?: boolean
}

function LazyElement({ routeElement, isPrivate = false }: RouteElement) {
  const LazyComponent = lazy(routeElement)
  return (
    <Suspense fallback={<div>loading...</div>}>
      {isPrivate ? (
        <PrivateRoute>
          <LazyComponent />
        </PrivateRoute>
      ) : (
        <LazyComponent />
      )}
    </Suspense>
  )
}

function wrapRoutesWithLazy({ routes, isPrivate = false }: LazyRouteProps) {
  return routes?.map((route: Route) => ({
    path: route.path,
    element: <LazyElement routeElement={route.element} isPrivate={isPrivate} />
  }))
}

export default function useRouteElements() {
  const routeElements = [
    {
      path: '/',
      element: <LazyElement routeElement={() => import('./pages/Home')} />
    },
    {
      path: '*',
      element: <NotFoundPage />
    },
    {
      path: `${PATH_URL.sponsors}/form`,
      element: (
        <PrivateRoute>
          <AddSponsor />
        </PrivateRoute>
      )
    },
    {
      path: '/admin',
      element: <Navigate to={PATH_URL.sponsors} />
    },
    ...wrapRoutesWithLazy({ routes: PRIVATE_ROUTE, isPrivate: true })
  ]
  return useRoutes(routeElements)
}
