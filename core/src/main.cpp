#include <ciso646>

#include <pb/utility/guard.hpp>

#include <spdlog/spdlog.h>

#include <SDL3/SDL.h>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#ifdef _WIN32
#include <Windows.h>
#endif

auto main() noexcept -> int
{
	using pb::infra::utility::Guard;

#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif

	// ==============================================
	// SDL
	// ==============================================

	if (not SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
	{
		SPDLOG_ERROR("[SDL] 初始化失败! {}", SDL_GetError());
		return -1;
	}
	const auto quit = Guard<void, &SDL_Quit>{};

	constexpr int window_width = 1920;
	constexpr int window_height = 1080;

	constexpr float window_logical_scale = 1.f;
	constexpr int window_logical_width = static_cast<int>(window_width * window_logical_scale);
	constexpr int window_logical_height = static_cast<int>(window_height * window_logical_scale);

	const auto window = Guard<SDL_Window, &SDL_DestroyWindow>{
			SDL_CreateWindow(
				PB_PROJECT_NAME " " PB_BUILD_TYPE " " PB_GIT_COMMIT_INFO,
				window_width,
				window_height,
				SDL_WINDOW_RESIZABLE
			)
	};
	if (window == nullptr)
	{
		SPDLOG_ERROR("[SDL] 创建窗口失败! {}", SDL_GetError());
		return -2;
	}

	const auto renderer = Guard<SDL_Renderer, &SDL_DestroyRenderer>{
			SDL_CreateRenderer(window, nullptr)
	};
	if (renderer == nullptr)
	{
		SPDLOG_ERROR("[SDL] 创建渲染器失败! {}", SDL_GetError());
		return -3;
	}

	// 设置渲染器支持透明色
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	// 设置垂直同步
	SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_ADAPTIVE);
	// 设置逻辑分辨率 (窗口大小 * 逻辑缩放比例)
	SDL_SetRenderLogicalPresentation(renderer, window_logical_width, window_logical_height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	SPDLOG_INFO("[SDL] 初始化完成!");

	// ==============================================
	// IMGUI
	// ==============================================


	IMGUI_CHECKVERSION();
	[[maybe_unused]] const auto imgui_context = Guard<ImGuiContext, &ImGui::DestroyContext>{ImGui::CreateContext()};

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	// io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; 

	ImGui::StyleColorsDark();

	if (not ImGui_ImplSDL3_InitForSDLRenderer(window, renderer))
	{
		SPDLOG_ERROR("[IMGUI] 初始化失败!");
		return -4;
	}
	const auto imgui_sdl3 = Guard<void, &ImGui_ImplSDL3_Shutdown>{};

	if (not ImGui_ImplSDLRenderer3_Init(renderer))
	{
		SPDLOG_ERROR("[IMGUI] 初始化失败!");
		return -5;
	}
	const auto imgui_renderer = Guard<void, &ImGui_ImplSDLRenderer3_Shutdown>{};

	if (auto* fonts = ImGui::GetIO().Fonts;
		fonts->AddFontFromFileTTF(
			R"(C:\Windows\Fonts\msyh.ttc)",
			16.f,
			nullptr,
			fonts->GetGlyphRangesChineseSimplifiedCommon())
		== nullptr
	)
	{
		SPDLOG_WARN("[IMGUI] 载入字体失败!");
	}
	else
	{
		ImGui_ImplSDLRenderer3_CreateFontsTexture();
	}

	SPDLOG_INFO("[IMGUI] 初始化完成!");

	bool should_close = false;
	while (not should_close)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);

			if (event.type == SDL_EVENT_QUIT)
			{
				should_close = true;
			}

			// 其他事件处理，如键盘、鼠标
			// 可以传递给 current_scene->handle_event(event);
		}

		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		// 更新场景
		// current_scene->update(delta_time);

		ImGui::ShowDemoWindow();
		ImGui::Begin("test");
		ImGui::Text("你好世界!");
		ImGui::End();

		// 渲染
		// 设置清屏颜色
		SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
		// 清屏
		SDL_RenderClear(renderer);

		ImGui::Render();
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

		// 交换缓冲区
		SDL_RenderPresent(renderer);
	}

	return 0;
}
