#include "stdafx.h"
#include "AdoInterface.h"


void CAdoInterface::ConnecDataLibrary(LPCTSTR ConnStr,LPCTSTR UserID,
		           LPCTSTR PassWord,long Options)
{
	m_pCon.CreateInstance(__uuidof(Connection));
	try
	{
		m_pCon->Open(_bstr_t(ConnStr),_bstr_t(UserID),
			                       _bstr_t(PassWord),Options);
	}
	catch(...)
	{
		AfxMessageBox("�������ݿ�ʧ�ܣ�");
	}
}

void CAdoInterface::CloseDataLibrary()
{
	m_pCon->Close();
}

void CAdoInterface::OpenSheet(LPCTSTR Sql,
		enum CursorTypeEnum CursorType,
		enum LockTypeEnum LockType,	long Options)
{
	m_pRec.CreateInstance(__uuidof(Recordset));
	try
	{		
		m_pRec->Open(Sql,m_pCon.GetInterfacePtr(),CursorType,LockType,Options);			
	}
	catch(...)
	{
		AfxMessageBox("�򿪱�ʧ�ܣ�");
	}
}

void CAdoInterface::CloseSheet()
{
	m_pRec->Close();
}

_RecordsetPtr CAdoInterface::Execute(LPCTSTR lpszSQL, long lOptions)
{
	ASSERT(m_pCon != NULL);
	ASSERT(AfxIsValidString(lpszSQL));
	try
	{
		return m_pCon->Execute(_bstr_t(lpszSQL), NULL, lOptions);
	}
	catch (...)
	{
		AfxMessageBox("ִ��SQL������");
	} 
	return FALSE;
}

BOOL CAdoInterface::GetCollect(LPCTSTR Name,_variant_t& OutCol)
{
	_variant_t vt;
	vt = m_pRec->GetCollect(Name);
	if(vt.vt != VT_NULL)
	{
		OutCol = vt;
		return TRUE;
	}
	else
		return FALSE;			
}

//----------------------��ֵ����-------------------------------
void CAdoInterface::AddNewRecode(AddNewCode* code,long ColCount)
{
	try
	{
		m_pRec->AddNew();
		for(int i=0;i<ColCount;++i)
			m_pRec->PutCollect(code[i].ColName,code[i].Value);

		m_pRec->Update();
	}
	catch(...)
	{
		AfxMessageBox("�����¼���ɹ���");
	}
}

void CAdoInterface::Delet(enum AffectEnum AffectRecords)
{
	m_pRec->Delete(AffectRecords);
	m_pRec->Update();
}

void CAdoInterface::GetOneRecord(_variant_t* ColName,long ColCount,
		                                 _variant_t* OutValue)
{
	try
	{
		for(int i=0;i<ColCount;++i)
		{
			OutValue[i] = m_pRec->GetCollect(ColName[i]);
		}
	}
	catch(...)
	{
		AfxMessageBox("��ȡһ����¼���ɹ���");
	}
}

BOOL CAdoInterface::Find(LPCTSTR lpszFind, 
		SearchDirectionEnum SearchDirection)
{
	ASSERT(m_pRec != NULL);
	ASSERT(AfxIsValidString(lpszFind));
	try
	{
		if (strcmp(lpszFind, _T("")) != 0)
		{
			m_strFind = lpszFind;
		}

		if (m_strFind.IsEmpty()) return FALSE;

		m_pRec->Find(_bstr_t(m_strFind), 0, SearchDirection, "");
		
		if ((adoEOF() || BOF()) )
		{
			return FALSE;
		}
		else
		{
			m_SearchDirection = SearchDirection;
			return TRUE;
		}
	}
	catch (...)
	{
		AfxMessageBox("Find����");
		return FALSE;
	}
}

BOOL CAdoInterface::FindNext()
{
	ASSERT(m_pRec != NULL);
	try
	{
		if (m_strFind.IsEmpty()) return FALSE;
		m_pRec->MoveNext();		
		if ((m_pRec->adoEOF || m_pRec->BOF) )
		{
			return FALSE;
		}
		else
		{
			m_pRec->Find(_bstr_t(m_strFind),0, m_SearchDirection);
			if ((m_pRec->adoEOF || m_pRec->BOF) )
			{
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
		
	}
	catch (...)
	{
		AfxMessageBox("FindNext����");
		return FALSE;
	}
}

BOOL CAdoInterface::Sort(LPCTSTR lpszSort)
{
	try
	{
		m_pRec->PutSort(lpszSort);
		return TRUE;
	}
	catch (...)
	{
		AfxMessageBox("Sort����");
		return FALSE;
	}
}

BOOL CAdoInterface::Filter(LPCTSTR lpszFilter)
{
	ASSERT(m_pRec != NULL);
	try
	{
		m_pRec->PutFilter(lpszFilter);
		return TRUE;
	}
	catch (...)
	{
		AfxMessageBox("Filter����");
		return FALSE;
	}
}

//----------------------��չ��װ����--------------------------------
void CAdoInterface::AddNewRecodeEx(CString ColName, ... )
{
	//˳��ȡ���ַ���ColName�е��ֶ������ֶ����ͣ����������vector��
	vector<FieldInfor> vfi;
	GetNameandType(ColName,vfi);	
	
    //��ָ���ַ���ColName�����ʡ�ԺŴ������в�����˳�����void*������
	vector<void*> vpn;
	void* pp = (&ColName) + (sizeof(ColName)/sizeof(CString));
	vpn.push_back(pp);
	for(int i=1;i<vfi.size();++i)
	{	
		if(vfi[i-1].Type == 4 || vfi[i-1].Type == 5)
		{
			void* pp = (double*)vpn[i-1] +      
				(sizeof(*(double*)vpn[i-1])/sizeof(double));
			vpn.push_back(pp);	
		}
		else
		{
			void* pp = (CString*)vpn[i-1] + 
				(sizeof(*(CString*)vpn[i-1])/sizeof(CString));
			vpn.push_back(pp);
		}		
	}
		
    //д����
	m_pRec->AddNew();
	for(int i=0;i<vfi.size();i++)
	{
		if(vfi[i].Type == 2 
			|| vfi[i].Type == 3 
			|| vfi[i].Type == 16 
			|| vfi[i].Type == 17
			|| vfi[i].Type == 18
			|| vfi[i].Type == 19
			|| vfi[i].Type == 20
			|| vfi[i].Type == 21)
		{
			m_pRec->PutCollect(vfi[i].Name , _variant_t(*(long*)vpn[i]));
		}
		else if(vfi[i].Type == 4 || vfi[i].Type == 5)
		{
			m_pRec->PutCollect(vfi[i].Name , _variant_t(*(double*)vpn[i]));
		}
		else
		{
			m_pRec->PutCollect(vfi[i].Name , _variant_t(*(CString*)vpn[i]));
		}
	}
	m_pRec->Update();    
}

void CAdoInterface::GetOneRecordEx(CString ColName, ... )
{
	//˳��ȡ���ַ���ColName�е��ֶ������ֶ����ͣ����������vector��
	vector<FieldInfor> vfi;
	GetNameandType(ColName,vfi);
	
	//��ָ���ַ���ColName�����ʡ�ԺŴ������в�����˳�����void**������
	vector<void**> vpn;
	void** pp = (void**)((&ColName) + (sizeof(ColName)/sizeof(CString)));
	vpn.push_back(pp);
	for(int i=1;i<vfi.size();++i)
	{
		void** pp = vpn[i-1] + 1;
		vpn.push_back(pp);
	}

	//��ȡ�ֶ����ݣ������ֶ����������⴫��ֵ
	for(int i=0;i<vfi.size();++i)
	{
		_variant_t var;
		var = m_pRec->GetCollect(vfi[i].Name);
		if(var.vt != VT_NULL)
		{
			if(vfi[i].Type == 2 
			|| vfi[i].Type == 3 
			|| vfi[i].Type == 16 
			|| vfi[i].Type == 17
			|| vfi[i].Type == 18
			|| vfi[i].Type == 19
			|| vfi[i].Type == 20
			|| vfi[i].Type == 21)
			{
				*((long*)(*vpn[i])) = var;				
			}
			else if(vfi[i].Type == 4 || vfi[i].Type == 5)
			{
				*((double*)(*vpn[i])) = var;
			}
			else
			{
				*((CString*)(*vpn[i])) = (LPCSTR)_bstr_t(var);
			}
		}		
	}
}

//---------------------------�㷨--------------------------------
void CAdoInterface::Ufo(CString InStr,vector<CString>& OutStr)
{
	vector<char> bug;
	vector<CString> newbug;
	vector<CString> result;
	for(int i=0;i<InStr.GetLength();++i)
	{
		if(InStr[i] != ',')
			bug.push_back(InStr[i]);
		else
		{
			CString sa;
			for(int i=0;i<bug.size();++i)
			{
				sa += bug[i];
			}
			result.push_back(sa);
			bug.clear();
		}		
	}
	CString sa;
	for(int i=0;i<bug.size();++i)
	{
		sa += bug[i];
	}
	result.push_back(sa);	
	OutStr = result;
}

void CAdoInterface::GetNameandType(CString ColName,vector<FieldInfor>& OutVnt)
{
	//��ָ���ַ���ColName�е��ֶ�����˳����ȡ�������У����һ�ȡ��Ӧ
	//�ֶε�������Ϣ,���㷨��Ҫ����һ��Name��Type�Ľṹ�������
	//1��ȡ��ĳ�������ֶ������ֶ�����
	FieldPtr fp;
	vector<FieldInfor> vfi;
	int Count = m_pRec->Fields->GetCount();
	for(int i=0;i<Count;++i)
	{
		FieldInfor fi;
		fp = m_pRec->Fields->GetItem(long(i));
		fi.Name = fp->Name;
		fi.Type = fp->Type;
		vfi.push_back(fi);
	}	
	//2����ָ���ַ���ColName�е��ֶ�����˳����ȡ��������
	vector<CString> vColName;
	Ufo(ColName,vColName);
	//3����ȫ���ֶ������ҳ����ַ���ColName���ֶ���������ֶΣ�����ͬ
	//���ֶε�����һͬ������ε�OutVnt��
	for(int i=0;i<vColName.size();++i)
	{
		for(int j=0;j<vfi.size();++j)
		{
			if(vColName[i] == (LPCTSTR)vfi[j].Name)
			{
				OutVnt.push_back(vfi[j]);				
			}									
		}
	}
}


