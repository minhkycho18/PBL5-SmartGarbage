import { Pagination, Tag } from 'antd'
import axios from 'axios'
import { useEffect, useState } from 'react'
import { Link, useNavigate } from 'react-router-dom'
import { useWebSocket } from 'react-use-websocket/dist/lib/use-websocket'
import CustomTable from 'src/components/common/CustomTable'
import HeaderPage from 'src/components/common/HeaderPage'
import { EVENTS } from 'src/data/events.dummy'
import AdminGuard from 'src/guard/AdminGuard'
import PATH_URL from 'src/shared/path'
import { Event } from 'src/types/events.type'

const columns = [
  {
    dataIndex: 'time',
    key: 'time',
    title: 'Time'
  },
  {
    dataIndex: 'status',
    key: 'status',
    title: 'Status',
    render: (status: string) => {
      return <span className={status === 'Clean' ? ' !text-green-500' : ''}>{status}</span>
    }
  },
  {
    dataIndex: 'type',
    key: 'type',
    render: (type: any) => {
      return type.id === 1 ? <Tag color='green'>Tái chế</Tag> : <Tag color='red'>Không tái chế</Tag>
    },
    title: 'Type'
  }
]

const getTrashColor = (message: string) => {
  if (message === 'Clean') return 'bg-green-400'
  if (message === 'Full') return 'bg-red-400'
}

export default function Notify() {
  const navigate = useNavigate()
  const [total, setTotal] = useState(0)
  const [page, setPage] = useState(1)
  const [garbages, setGarbages] = useState([])
  const [type, setType] = useState('')
  const [message, setMessage] = useState('')
  const setSelectedItem = (item: Event) => {
    console.log(item)
  }
  const handleDelete = () => {
    console.log('delete')
  }
  const onChange = () => {
    console.log('change')
  }
  const { sendMessage, lastMessage } = useWebSocket('ws://localhost:8000/ws/socket-server/')
  useEffect(() => {
    if (lastMessage !== null) {
      const { data, message, type_trash, time } = JSON.parse(lastMessage?.data)

      setType(type_trash)
      setMessage(message)
    }
  }, [lastMessage])

  useEffect(() => {
    const getData = async () => {
      const res = await axios.get(`http://localhost:8000/notify/?page=${page}`)
      setGarbages(res?.data?.results)
      setTotal(res.data?.count)
    }
    getData()
    console.log(garbages.filter((item: any) => item.type.id === 'Recyclable')?.[0])
    console.log(garbages.filter((item: any) => item.type.id === 'Non-Recyclable')?.[0])
  }, [page, lastMessage])

  const handleChangePage = (val: number) => {
    setPage(val)
  }
  return (
    <AdminGuard>
      <HeaderPage
        title='Thông báo'
        onCreate={() => {
          navigate(`${PATH_URL.sponsors}/form`)
        }}
        breadcrumbList={[]}
      />
      <div className='my-2 flex items-center justify-center text-2xl font-bold'>
        <span>Tình trạng thùng rác</span>
      </div>
      <div className='mb-4 flex justify-center'>
        <div
          className={` mr-3 flex h-32 w-20 ${
            (garbages.filter((item: any) => item.type.name === 'Recyclable')?.[0] as any)?.status === 'Full'
              ? 'bg-red-400'
              : 'bg-green-400'
          } items-center justify-center bg-green-500 `}
        >
          Tái chế
        </div>
        <div
          className={`flex h-32 w-20 ${
            (garbages.filter((item: any) => item.type.name === 'Non-Recyclable')?.[0] as any)?.status === 'Full'
              ? 'bg-red-400'
              : 'bg-green-400'
          } items-center justify-center bg-green-500 text-center `}
        >
          Không tái chế
        </div>
      </div>

      <CustomTable<Event>
        columns={columns}
        currentPage={1}
        dataSource={garbages}
        pageSize={10}
        total={40}
        onChange={onChange}
        loading={false}
      />
      <Pagination total={total} defaultCurrent={page} onChange={handleChangePage} />
    </AdminGuard>
  )
}
