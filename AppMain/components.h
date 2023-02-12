#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <ImGui/imgui.h>
#include <NodeEditor/imgui_node_editor.h>
#include <mutex>
#include <string>
#include <vector>
#include <map>
#include <functional>

using namespace ImGui;
namespace im = ImGui;
namespace ed = ax::NodeEditor;

#define SCOPED_LOCK() std::lock_guard<std::mutex> _lock(m_Mutex)

class ElementInfo;
class GraphInfo;
class NodeInfo;
class PinInfo;
class LinkInfo;

typedef std::vector<NodeInfo*> NodeList;
typedef std::vector<LinkInfo*> LinkList;
typedef std::vector<PinInfo*> PinList;

template<class E>
using Builder = std::function<void(E*)>;

class ElementInfo
{
public:
	using Super = ElementInfo;

	inline ElementInfo(GraphInfo* graph, uint32_t id) :
		m_Graph(graph),
		m_Id(id)
	{

	}

	inline uint32_t GetId() {
		return m_Id;
	}

	inline GraphInfo* GetGraph() const {
		return m_Graph;
	}

private:
	GraphInfo* m_Graph;
	uint32_t m_Id;
};

class GraphInfo
{
private:
	volatile uint32_t m_Counter;
	NodeList m_Nodes;
	LinkList m_Links;
	std::mutex m_Mutex;

public:
	NodeInfo* CreateNode();
	uint32_t GetNextId();
	void OnRender();

	inline GraphInfo() :
		m_Counter(0),
		m_Nodes(),
		m_Links(),
		m_Mutex()
	{

	}

	GraphInfo(const GraphInfo& ref) = delete;
};


class LinkInfo : public ElementInfo
{
private:
	uint32_t m_NodeId;
	uint32_t m_StartPin;
	uint32_t m_EndPin;

public:
	LinkInfo(GraphInfo* graph, uint32_t id, uint32_t nodeId, uint32_t start, uint32_t end) :
		ElementInfo(graph, id),
		m_NodeId(nodeId),
		m_StartPin(start),
		m_EndPin(end)
	{

	}

	void OnRender();
};

class NodeInfo : public ElementInfo
{
private:
	GraphInfo m_Graph;
	PinList m_InputPins;
	PinList m_OutputPins;
	std::string m_Title;
	std::string m_Description;
	std::mutex m_Mutex;

public:
	NodeInfo(GraphInfo* graph, uint32_t id) :
		ElementInfo(graph, id)
	{

	}

	void AddInputPin(Builder<PinInfo> fn);
	void AddOutputPin(Builder<PinInfo> fn);
	void AddInputPin(PinInfo* pin);
	void AddOutputPin(PinInfo* pin);
	void RemovePin(uint32_t id);
	void RemovePin(PinInfo* pin);
	void OnRender();

	uint32_t AddPin(const std::string& text, ed::PinKind kind);
	void SetTitle(const std::string& value);
	void SetDescription(const std::string& value);
};

class PinInfo : public ElementInfo
{
private:
	NodeInfo* m_Node;
	ed::PinKind m_Kind;
	std::string m_Text;

public:
	PinInfo(GraphInfo* graph, uint32_t id, NodeInfo* node, ed::PinKind kind) :
		ElementInfo(graph, id),
		m_Node(node),
		m_Kind(kind)
	{

	}

	inline const ed::PinKind GetKind()
	{
		return m_Kind;
	}

	PinInfo(GraphInfo* graph, NodeInfo* node, uint32_t id, ed::PinKind kind) :
		ElementInfo(graph, id),
		m_Node(node),
		m_Kind(kind)
	{

	}

	inline const NodeInfo* GetNode() {
		return m_Node;
	}

	void SetText(const std::string& text);
	void SetText(const char* text);
	void OnRender();
};

#endif 