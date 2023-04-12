import { Tag } from 'antd'
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
    dataIndex: 'name',
    key: 'name',
    title: 'Name'
  },
  {
    dataIndex: 'date',
    key: 'date',
    title: 'date'
  },
  {
    dataIndex: 'type',
    key: 'type',
    render: (type: any) => (type.id === 1 ? <Tag title='Tái chế' /> : <Tag title='Không tái chế' />),
    title: 'type'
  }
]

export default function Sponsors() {
  const navigate = useNavigate()
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
      const res = await axios.get('http://localhost:8000/images/?date=2023-04-06&page=2')
      console.log(res)
      setGarbages(res.data.result)
    }
    getData()
  })

  return (
    <AdminGuard>
      <HeaderPage
        title='Rác'
        onCreate={() => {
          navigate(`${PATH_URL.sponsors}/form`)
        }}
        breadcrumbList={[]}
      />
      <CustomTable<Event>
        columns={columns}
        currentPage={1}
        dataSource={garbages}
        onDelete={handleDelete}
        onEdit={setSelectedItem}
        pageSize={10}
        total={40}
        onChange={onChange}
        loading={false}
      />
    </AdminGuard>
  )
}
