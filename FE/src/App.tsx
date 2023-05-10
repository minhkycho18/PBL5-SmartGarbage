import { ToastContainer, toast } from 'react-toastify'
import useRouteElements from './useRoutesElement'
import 'react-toastify/dist/ReactToastify.css'

import useWebSocket from 'react-use-websocket'
import { useEffect } from 'react'

function App() {
  const routeElements = useRouteElements()
  const { sendMessage, lastMessage } = useWebSocket('ws://localhost:8000/ws/socket-server/')
  useEffect(() => {
    if (lastMessage !== null) {
      console.log(lastMessage, 'lastmsg')
      const { data, message, type_trash, time } = JSON.parse(lastMessage?.data)
      console.log(message, data)
      toast.success(`${type_trash} trash was ${message}ed at ${time} `, {
        position: 'top-right',
        autoClose: 2000
      })
    }
  }, [lastMessage])

  return (
    <>
      {routeElements}
      <ToastContainer />
    </>
  )
}

export default App
