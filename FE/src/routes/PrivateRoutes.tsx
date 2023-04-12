import { useEffect, useState } from 'react'
import PrivateLayout from 'src/components/layouts/PrivateLayout'
import { ReactWithChild } from 'src/interface/app'

function PrivateRoute({ children }: ReactWithChild) {
  const [havePermission, setHavePermission] = useState(false) // Sau ni dung accessToken

  useEffect(() => {
    //handle redirect o day
  }, [])

  return <PrivateLayout>{children}</PrivateLayout>
}

export default PrivateRoute
