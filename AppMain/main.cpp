#include <AppCore/application.h>
#include "components.h"
#include <memory>

const char* s_AppClass = "OverwatchVisualScriptApplication";
const char* s_AppTitle = "Overwatch: Visual Scripting";

static std::shared_ptr<GraphInfo> g_pGraph = nullptr;
static ed::EditorContext* g_pContext = nullptr;

void WINAPI Application_OnInit(int argc, char** argv)
{
	g_pGraph = std::make_shared<GraphInfo>();
	{
		auto* node = g_pGraph->CreateNode();
		node->SetTitle("Logic:ManyToMany");
		node->SetDescription("Peform a many to many operation and return respective results");
		node->AddPin("In 1", ed::PinKind::Input);
		node->AddPin("In 2", ed::PinKind::Input);
		node->AddPin("In 3", ed::PinKind::Input);
		node->AddPin("Out 1", ed::PinKind::Output);
		node->AddPin("Out 2", ed::PinKind::Output);
		node->AddPin("Out 3", ed::PinKind::Output);
	}

	g_pContext = ed::CreateEditor();

	ed::SetCurrentEditor(g_pContext);
}

static bool bIsContextMenuOpen = false;

void WINAPI Application_OnRender()
{
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("##DockspaceFrame", nullptr, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("OverPrintsDockSpaceControl");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), 0);
	}

	{
		if (ImGui::Begin("Node Editor", 0))
		{
			ed::Begin("Editor");
			{
				if (g_pGraph)
					g_pGraph->OnRender();
			}
			ed::End();
		}
		ImGui::End();

		ImGui::End();
	}

	im::BeginMainMenuBar();

	if (im::BeginMenu("File"))
	{
		if (im::MenuItem("Quit"))
			Quit(0);

		im::EndMenu();
	}

	im::EndMainMenuBar();
}