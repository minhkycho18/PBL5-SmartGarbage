import { Pagination, Tag } from 'antd'
import axios from 'axios'
import { useEffect, useState } from 'react'
import { Link, useNavigate } from 'react-router-dom'
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

export default function Notify() {
  const navigate = useNavigate()
  const [total, setTotal] = useState(0)
  const [page, setPage] = useState(1)
  const [garbages, setGarbages] = useState([])
  const setSelectedItem = (item: Event) => {
    console.log(item)
  }
  const handleDelete = () => {
    console.log('delete')
  }
  const onChange = () => {
    console.log('change')
  }

  useEffect(() => {
    const getData = async () => {
      const res = await axios.get(`http://localhost:8000/notify/?page=${page}`)
      setGarbages(res?.data?.results)
      setTotal(res.data?.count)
    }
    getData()
  }, [page])

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
