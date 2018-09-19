#pragma once
#include <string>
#include "sqlliteRW.h"

struct CENTRE
{
	double x;
	double y;
	double z;
};

class BGetOneDbData
{
public:
	BGetOneDbData(const std::string& ip, const std::string& port);
	~BGetOneDbData();
	void SetUorM(WString& wstr);

	void getOneModel();
	void creatFiles();
	void toTransform(std::string str, double myTransform[4][4]);
	void getCentre();
	void findAllElements(std::string& oldFile, std::string& oldModel, std::string& thisFileName, std::string& thisModelName, std::string& transform, std::string& logicalName, std::string& modelId);

	void callbackMsg(int& i, sqlliteRW& sqlW, WString& dbName, ElementHandleR sourceEh, std::string& tableElementElemGuidInfile, std::string& referFile, double myTransform[4][4]);

	void writeMsg(int& i, sqlliteRW& sqlW, WString& dbName, ElementHandleR sourceEh, std::string& pros, WString& name, std::string& tableElementElemGuidInfile, std::string& referFile, double myTransform[4][4]);


	////获取元素所有属性
	WString doShowECInstance(ElementHandleR elem);
	std::string findOne(const std::string& str, const std::string& find);


	PROPERTRY properties;
	void getProperties(std::string& str);



	bool creatReference(ElementHandleR sourceEh, WString modelNamel, sqlliteRW& sqlW, double myTransform[4][4], std::string& dataStr, std::string & pic, std::string & mtlName, bool& colorFlag);

	WString get(PolyfaceHeaderPtr meshData, double myTransform[4][4], const std::string& colorName, sqlliteRW& sqlW);
	bool ElementToApproximateFacets(ElementHandleCR source, bvector<PolyfaceHeaderPtr> &output, IFacetOptionsP options);

	std::string materialInfo(ElementHandleR sourceEh);
	


	static void compress(const std::string& src);


	std::string ip;
	std::string port;

	std::string path;
	CENTRE centre;
};


struct MeshProcessor : public IElementGraphicsProcessor
{
	bvector <PolyfaceHeaderPtr> &m_output;
	IFacetOptionsP               m_options;
	Transform                    m_currentTransform;

	MeshProcessor(bvector<PolyfaceHeaderPtr> &output, IFacetOptionsP options)
		: m_output(output), m_options(options)
	{
	}

	virtual IFacetOptionsP _GetFacetOptionsP()
	{
		return m_options;
	}

	virtual bool _ProcessAsFacets(bool isPolyface) const override
	{
		return true;
	}

	virtual bool _ProcessAsBody(bool isCurved) const
	{
		return false;
	}

	virtual void _AnnounceTransform(TransformCP trans) override
	{
		if (trans)
			m_currentTransform = *trans;
		else
			m_currentTransform.InitIdentity();
	}


	virtual BentleyStatus _ProcessFacets(PolyfaceQueryCR facets, bool isFilled = false) override
	{
		PolyfaceHeaderPtr header = PolyfaceHeader::CreateTriangleGrid(3);
		header->CopyFrom(facets);
		header->Transform(m_currentTransform);
		m_output.push_back(header);
		return SUCCESS;
	}
};

