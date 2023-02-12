#include "components.h"
#include <ImGui/imgui_internal.h>
#include <algorithm>
#include <map>

uint32_t GraphInfo::GetNextId()
{
	uint32_t value;

	{
		SCOPED_LOCK();
		value = ++m_Counter;
	}

	return value;
}

NodeInfo* GraphInfo::CreateNode()
{
	auto* node = new NodeInfo(this, GetNextId());

	{
		SCOPED_LOCK();
		m_Nodes.push_back(node);
	}

	return node;
}

void GraphInfo::OnRender()
{
	SCOPED_LOCK();

	for (NodeInfo* node : m_Nodes)
		node->OnRender();

	for (LinkInfo* link : m_Links)
		link->OnRender();
}

void LinkInfo::OnRender()
{
	ed::Link(GetId(), m_StartPin, m_EndPin);
}

void PinInfo::SetText(const std::string& text)
{
	m_Text = text;
}

void PinInfo::SetText(const char* text)
{
	m_Text = text;
}

template<class T>
bool EraseById(T* v, uint32_t id)
{
	T::value_type result = nullptr;
	T::const_iterator it;

	for (it = v->begin(); it != v->end(); ++it)
	{
		T::value_type value = (T::value_type) & *it;

		if (value->GetId() == id) {
			result = value;
			break;
		}
	}

	if (it != v->end()) {
		v->erase(it);

		if (result)
			delete result;

		return true;
	}

	return false;
}

void NodeInfo::RemovePin(uint32_t id)
{
	SCOPED_LOCK();

	if (!EraseById(&m_InputPins, id))
		EraseById(&m_OutputPins, id);
}

void NodeInfo::RemovePin(PinInfo* pin)
{
	PinList* list = nullptr;

	if (pin->GetKind() == ed::PinKind::Input)
		list = &m_InputPins;
	else if (pin->GetKind() == ed::PinKind::Output)
		list = &m_OutputPins;

	SCOPED_LOCK();
	auto where = std::find(list->begin(), list->end(), pin);

	if (where != list->end()) {
		list->erase(where);
	}

	delete pin;
}

void NodeInfo::OnRender()
{
	ed::BeginNode(GetId());

	if (!m_Title.empty())
		im::TextUnformatted(m_Title.c_str());

	if (!m_Description.empty())
		im::TextUnformatted(m_Description.c_str());

	char temp[32];
	sprintf(temp, "node_%l_table", GetId());

	SCOPED_LOCK();

	unsigned cols = 2,
		rows = std::max(m_InputPins.size(), m_OutputPins.size());

	auto size = ed::GetNodeSize(GetId());
	im::SetNextItemWidth(size.x);

	im::BeginTable(temp, cols);

	for (size_t n = 0; n < rows; n++)
	{
		if (n <= m_InputPins.size() - 1)
			m_InputPins[n]->OnRender();

		if (n <= m_OutputPins.size() - 1)
			m_OutputPins[n]->OnRender();
	}
	im::EndTable();

	ed::EndNode();
}

void PinInfo::OnRender()
{
	ed::BeginPin(GetId(), m_Kind);

	if (m_Kind == ed::PinKind::Output)
	{
		im::TableSetColumnIndex(1);
		im::TextUnformatted(m_Text.c_str());
	}
	else
	{
		im::TableSetColumnIndex(0);
		im::TextUnformatted(m_Text.c_str());
	}

	ed::EndPin();
}

void NodeInfo::SetTitle(const std::string& value)
{
	m_Title = value;
}

void NodeInfo::SetDescription(const std::string& value)
{
	m_Description = value;
}

void NodeInfo::AddInputPin(Builder<PinInfo> fn)
{
	auto* graph = GetGraph();
	auto id = graph->GetNextId();
	auto pin = new PinInfo(graph, this, id, ed::PinKind::Input);

	fn(pin);

	SCOPED_LOCK();
	m_InputPins.push_back(pin);
}

uint32_t NodeInfo::AddPin(const std::string& text, ed::PinKind kind)
{
	auto* graph = GetGraph();
	auto id = graph->GetNextId();

	auto* pin = new PinInfo(graph, this, id, kind);
	pin->SetText(text);

	{
		SCOPED_LOCK();

		if (kind == ed::PinKind::Input)
			m_InputPins.push_back(pin);
		else if (kind == ed::PinKind::Output)
			m_OutputPins.push_back(pin);
		else
			assert(!"Invalid pin kind! Must be either \"INPUT\" or \"OUTPUT\"");
	}

	return id;
}

void NodeInfo::AddOutputPin(Builder<PinInfo> fn)
{
	auto* graph = GetGraph();
	auto id = graph->GetNextId();
	auto pin = new PinInfo(graph, this, id, ed::PinKind::Output);

	fn(pin);

	SCOPED_LOCK();
	m_OutputPins.push_back(pin);
}

void NodeInfo::AddInputPin(PinInfo* pin)
{
	SCOPED_LOCK();
	m_InputPins.push_back(pin);
}

void NodeInfo::AddOutputPin(PinInfo* pin)
{
	SCOPED_LOCK();
	m_OutputPins.push_back(pin);
}