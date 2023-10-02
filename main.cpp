#include "includes.h"
#include "SDK.h"

using namespace Dumper;

void console_Initialize() {
	AllocConsole();
	freopen_s(reinterpret_cast<FILE**> stdout, "CONOUT$", "w", stdout);
}

void console_Shutdown() {
	fclose(stdout);
	FreeConsole();
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;
static bool bBone = true,bBox,bHealth, bAimBot, bDistance, bVisible = true,bShowRange, bDrawLine, bSlient, bDrawName;

static FLinearColor BoxColor = { 1.f, 1.f, 1.f ,1.f };


void DefaultTheme() {

	ImGuiStyle& s = ImGui::GetStyle();

	const ImColor accentCol = ImColor(255, 0, 0, 255);
	const ImColor bgSecondary = ImColor(255, 0, 0, 255);
	s.Colors[ImGuiCol_WindowBg] = ImColor(32, 32, 32, 255);
	s.Colors[ImGuiCol_ChildBg] = bgSecondary;
	s.Colors[ImGuiCol_FrameBg] = ImColor(65, 64, 64, 255);
	s.Colors[ImGuiCol_FrameBgActive] = ImColor(35, 37, 39, 255);
	s.Colors[ImGuiCol_Border] = ImColor(1, 1, 1, 255);
	s.Colors[ImGuiCol_CheckMark] = ImColor(255, 0, 0, 255);
	s.Colors[ImGuiCol_SliderGrab] = ImColor(255, 0, 0, 255);
	s.Colors[ImGuiCol_SliderGrab] = ImColor(255, 0, 0, 255);
	s.Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 0, 0, 255);
	s.Colors[ImGuiCol_ResizeGrip] = ImColor(24, 24, 24, 255);
	s.Colors[ImGuiCol_Header] = ImColor(0, 0, 0, 255);
	s.Colors[ImGuiCol_HeaderHovered] = ImColor(0, 0, 0, 255);
	s.Colors[ImGuiCol_HeaderActive] = ImColor(0, 0, 0, 255);
	s.Colors[ImGuiCol_TitleBg] = ImColor(0, 0, 0, 255);
	s.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(0, 0, 0, 255);
	s.Colors[ImGuiCol_TitleBgActive] = ImColor(0, 0, 0, 255);
	s.Colors[ImGuiCol_FrameBgHovered] = ImColor(65, 64, 64, 255);
	s.Colors[ImGuiCol_ScrollbarBg] = ImColor(0, 0, 0, 255);
	s.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	s.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	s.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	s.Colors[ImGuiCol_Header] = ImColor(42, 42, 42, 255);
	s.Colors[ImGuiCol_HeaderHovered] = ImColor(50, 50, 50, 255);
	s.Colors[ImGuiCol_HeaderActive] = ImColor(50, 50, 50, 255);
	s.Colors[ImGuiCol_PopupBg] = ImColor(15, 15, 15, 255);
	s.Colors[ImGuiCol_Button] = ImColor(30, 30, 30, 255);
	s.Colors[ImGuiCol_ButtonHovered] = ImColor(30, 30, 30, 255);
	s.Colors[ImGuiCol_ButtonActive] = ImColor(30, 30, 30, 255);
	s.Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);

}

void HotkeyButton(USHORT& dst, const char* const label, const char* new_button_id, bool& get_key)
{
#define XorCStr(x) x

	std::string button;

	if (get_key)
		button += "...";
	else
	{
		switch (dst)
		{
		case 0:
			button += XorCStr("NONE");
			break;
		case VK_ESCAPE:
			button += XorCStr("ESC");
			break;
		case VK_TAB:
			button += XorCStr("TAB");
			break;
		case VK_CAPITAL:
			button += XorCStr("CAPS");
			break;
		case VK_SHIFT:
			button += XorCStr("SHIFT");
			break;
		case VK_CONTROL:
			button += XorCStr("CTRL");
			break;
		case VK_MENU:
			button += XorCStr("ALT");
			break;
		case VK_XBUTTON1:
			button += XorCStr("MB 1");
			break;
		case VK_XBUTTON2:
			button += XorCStr("MB 2");
			break;
		case VK_F1:
			button += XorCStr("F1");
			break;
		case VK_F2:
			button += XorCStr("F2");
			break;
		case VK_F3:
			button += XorCStr("F3");
			break;
		case VK_F4:
			button += XorCStr("F4");
			break;
		case VK_F5:
			button += XorCStr("F5");
			break;
		case VK_F6:
			button += XorCStr("F6");
			break;
		case VK_F7:
			button += XorCStr("F7");
			break;
		case VK_F8:
			button += XorCStr("F8");
			break;
		case VK_F9:
			button += XorCStr("F9");
			break;
		case VK_F10:
			button += XorCStr("F10");
			break;
		case VK_F11:
			button += XorCStr("F11");
			break;
		case VK_F12:
			button += XorCStr("F12");
			break;
		case VK_F13:
			button += XorCStr("F13");
			break;
		case VK_F14:
			button += XorCStr("F14");
			break;
		case VK_F15:
			button += XorCStr("F15");
			break;
		case VK_F16:
			button += XorCStr("F16");
			break;
		case VK_F17:
			button += XorCStr("F17");
			break;
		case VK_F18:
			button += XorCStr("F18");
			break;
		case VK_F19:
			button += XorCStr("F19");
			break;
		case VK_F20:
			button += XorCStr("F20");
			break;
		case VK_F21:
			button += XorCStr("F21");
			break;
		case VK_F22:
			button += XorCStr("F22");
			break;
		case VK_F23:
			button += XorCStr("F23");
			break;
		case VK_F24:
			button += XorCStr("F24");
			break;
		case VK_HOME:
			button += XorCStr("HOME");
			break;
		case VK_DELETE:
			button += XorCStr("DELETE");
			break;
		case VK_INSERT:
			button += XorCStr("INSERT");
			break;
		case VK_END:
			button += XorCStr("END");
			break;
		case VK_SPACE:
			button += XorCStr("SPACE");
			break;
		case VK_SCROLL:
			button += XorCStr("SCROLL");
			break;
		case VK_LBUTTON:
			button += XorCStr("LCLICK");
			break;
		case VK_RBUTTON:
			button += XorCStr("RCLICK");
			break;
		case VK_RETURN:
			button += XorCStr("ENTER");
			break;
		default:
			button += dst;
		}
	}
	button += new_button_id;

	if (ImGui::Button(button.c_str(), ImVec2(70, 0)))
		get_key = true;
	if (ImGui::IsItemHovered())
	{
		if (dst != 0xADAF)
			ImGui::SetTooltip(XorCStr("Double Click to Reset"));
		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			get_key = false;
			dst = 0;
		}
	}

	ImGui::SameLine();
	ImGui::Text(label);

	if (get_key)
	{
		for (int i = 0x1; i <= 0x100; i += 0x1)
		{
			if (GetAsyncKeyState(i) & 0x8000)
			{
				get_key = false;
				dst = i;
				break;
			}
		}
	}

#undef XorCStr
}

USHORT aimKey = VK_CAPITAL;
static bool satg = false;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}


void DrawBox(FVector2D& topleft, FVector2D& downright, ImU32 clr)
{
	auto h = downright.Y - topleft.Y;
	auto w = downright.X - topleft.X;

	auto downleft = FVector2D{ topleft.X, downright.Y };
	auto topright = FVector2D{ downright.X, topleft.Y };

	auto thicc = 1.f;

	//FLinearColor xclr = RGBtoFLC(0, 0, 0);
	ImGui::GetForegroundDrawList()->AddLine({ topleft.X,topleft.Y }, { downright.X, topleft.Y }, clr);
	ImGui::GetForegroundDrawList()->AddLine({ topleft.X,topleft.Y }, { topleft.X , downright.Y }, clr);
	ImGui::GetForegroundDrawList()->AddLine({ downright.X,downright.Y }, { topleft.X , downright.Y }, clr);
	ImGui::GetForegroundDrawList()->AddLine({ downright.X,downright.Y }, { downright.X, topleft.Y }, clr);

}

void DrawLine(FVector2D St, FVector2D Ed, float ti, FLinearColor col) {
	ImGui::GetForegroundDrawList()->
		AddLine({ St.X,St.Y }, { Ed.X,Ed.Y }, ImColor{ col.R,col.G,col.B,col.A }, ti);
}



namespace Bones {
    FName Head, Neck, Spine, Pelvis;
    FName L_Clavicle, L_UpperArm, L_Forearm, L_Hand;
    FName R_Clavicle, R_UpperArm, R_Forearm, R_Hand;
    FName L_Thigh, L_Calf, L_Foot;
    FName R_Thigh, R_Calf, R_Foot;

    static std::vector<FName>Loop;
	static FLinearColor BonesColor = { 0.f, 1.f, 0.f ,1.f };

    void Init() {
		auto stringlib = (UKismetStringLibrary*)UKismetStringLibrary::StaticClass();
		
        Head = stringlib->STATIC_Conv_StringToName(L"Bip001-Head");
        Neck = stringlib->STATIC_Conv_StringToName(L"Bip001-Neck");
        Spine = stringlib->STATIC_Conv_StringToName(L"Bip001-Spine");
        Pelvis = stringlib->STATIC_Conv_StringToName(L"Bip001-Pelvis");

        L_Clavicle = stringlib->STATIC_Conv_StringToName(L"Bip001-L-Clavicle");
        L_UpperArm = stringlib->STATIC_Conv_StringToName(L"Bip001-L-UpperArm");
        L_Forearm = stringlib->STATIC_Conv_StringToName(L"Bip001-L-Forearm");
        L_Hand = stringlib->STATIC_Conv_StringToName(L"Bip001-L-Hand");
        R_Clavicle = stringlib->STATIC_Conv_StringToName(L"Bip001-R-Clavicle");
        R_UpperArm = stringlib->STATIC_Conv_StringToName(L"Bip001-R-UpperArm");
        R_Forearm = stringlib->STATIC_Conv_StringToName(L"Bip001-R-Forearm");
        R_Hand = stringlib->STATIC_Conv_StringToName(L"Bip001-R-Hand");

        L_Thigh = stringlib->STATIC_Conv_StringToName(L"Bip001-L-Thigh");
        L_Calf = stringlib->STATIC_Conv_StringToName(L"Bip001-L-Calf");
        L_Foot = stringlib->STATIC_Conv_StringToName(L"Bip001-L-Foot");
        R_Thigh = stringlib->STATIC_Conv_StringToName(L"Bip001-R-Thigh");
        R_Calf = stringlib->STATIC_Conv_StringToName(L"Bip001-R-Calf");
        R_Foot = stringlib->STATIC_Conv_StringToName(L"Bip001-R-Foot");

        Loop = std::vector<FName>{ Head,Neck,Pelvis,L_Clavicle,L_UpperArm,R_Clavicle,R_UpperArm,L_Thigh,L_Calf,R_Thigh,R_Calf,L_Foot,R_Foot };
    }

    void Draw(APlayerController* Controller, USkeletalMeshComponent* Mesh) {
        FVector HeadPos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(Head));
        FVector NeckPos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(Neck));
        FVector SpinePos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(Spine));
        FVector PelvisPos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(Pelvis));
        FVector L_ClaviclePos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(L_Clavicle));
        FVector L_UpperArmPos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(L_UpperArm));
        FVector L_ForearmPos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(L_Forearm));
        FVector L_HandPos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(L_Hand));
        FVector R_ClaviclePos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(R_Clavicle));
        FVector R_UpperArmPos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(R_UpperArm));
        FVector R_ForearmPos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(R_Forearm));
        FVector R_HandPos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(R_Hand));
        FVector L_ThighPos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(L_Thigh));
        FVector L_CalfPos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(L_Calf));
        FVector L_FootPos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(L_Foot));
        FVector R_ThighPos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(R_Thigh));
        FVector R_CalfPos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(R_Calf));
        FVector R_FootPos = Mesh->GetBoneWorldPos(Mesh->GetBoneIndex(R_Foot));
        FVector2D Head_2D, Neck_2D, Spine_2D, Pelvis_2D;
        FVector2D L_Clavicle_2D, L_UpperArm_2D, L_Forearm_2D, L_Hand_2D;
        FVector2D R_Clavicle_2D, R_UpperArm_2D, R_Forearm_2D, R_Hand_2D;
        FVector2D L_Thigh_2D, L_Calf_2D, L_Foot_2D;
        FVector2D R_Thigh_2D, R_Calf_2D, R_Foot_2D;
        if (!Controller->ProjectWorldLocationToScreen(HeadPos, Head_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(NeckPos, Neck_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(SpinePos, Spine_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(PelvisPos, Pelvis_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(L_ClaviclePos, L_Clavicle_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(L_UpperArmPos, L_UpperArm_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(L_ForearmPos, L_Forearm_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(L_HandPos, L_Hand_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(R_ClaviclePos, R_Clavicle_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(R_UpperArmPos, R_UpperArm_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(R_ForearmPos, R_Forearm_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(R_HandPos, R_Hand_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(L_ThighPos, L_Thigh_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(L_CalfPos, L_Calf_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(L_FootPos, L_Foot_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(R_ThighPos, R_Thigh_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(R_CalfPos, R_Calf_2D, false))return;
        if (!Controller->ProjectWorldLocationToScreen(R_FootPos, R_Foot_2D, false))return;

       DrawLine(Head_2D, Neck_2D, 2.0, BonesColor);
       DrawLine(Neck_2D, Pelvis_2D, 2.0, BonesColor);
       DrawLine(Neck_2D, L_Clavicle_2D, 2.0, BonesColor);
       DrawLine(L_Clavicle_2D, L_UpperArm_2D, 2.0, BonesColor);
       DrawLine(L_UpperArm_2D, L_Forearm_2D, 2.0, BonesColor);
       DrawLine(L_Forearm_2D, L_Hand_2D, 2.0, BonesColor);
       DrawLine(Neck_2D, R_Clavicle_2D, 2.0, BonesColor);
       DrawLine(R_Clavicle_2D, R_UpperArm_2D, 2.0, BonesColor);
       DrawLine(R_UpperArm_2D, R_Forearm_2D, 2.0, BonesColor);
       DrawLine(R_Forearm_2D, R_Hand_2D, 2.0, BonesColor);
       DrawLine(Pelvis_2D, L_Thigh_2D, 2.0, BonesColor);
       DrawLine(L_Thigh_2D, L_Calf_2D, 2.0, BonesColor);
       DrawLine(L_Calf_2D, L_Foot_2D, 2.0, BonesColor);
       DrawLine(Pelvis_2D, R_Thigh_2D, 2.0, BonesColor);
       DrawLine(R_Thigh_2D, R_Calf_2D, 2.0, BonesColor);
       DrawLine(R_Calf_2D, R_Foot_2D, 2.0, BonesColor);

    }

}

int aimpos = 8;

int style_idx;
static float AimRange = 150, _smess = 3;

FVector2D winCenter;

FName Aimto;

bool Trace(TArray<AActor*> Ig,FVector& Start, FVector& End) {
	UKismetSystemLibrary* KismetSystemLibrary = (UKismetSystemLibrary*)KismetSystemLibrary->StaticClass();
	if (!KismetSystemLibrary)return 0;

	FHitResult hit;
	auto ret = KismetSystemLibrary->STATIC_LineTraceSingle(
		*UWorld::GWorld,
		Start,
		End,
		ETraceTypeQuery::TraceTypeQuery1,
		true,
		Ig,
		EDrawDebugTrace::None,
		hit,
		true,
		{ 0,0,0,0 },
		{ 0,0,0,0 },
		0.0f
	);

	return ret;
}


void Tick(){

	auto World = *UWorld::GWorld;
	float dis = 114514.f;
	AWFBaseCharacter* Target = nullptr;
	if (!World->OwningGameInstance)return;
	auto GameInstance = World->OwningGameInstance;
	if (!GameInstance->LocalPlayers[0])return;
	auto LocalPlayer = GameInstance->LocalPlayers[0];
	if (!LocalPlayer->PlayerController)return;
	auto PlayerController = LocalPlayer->PlayerController;
	if (!PlayerController->AcknowledgedPawn)return;
	auto AcknowledgedPawn = (AWFBaseCharacter*)PlayerController->AcknowledgedPawn;
	if (!AcknowledgedPawn->PlayerState)return;
	auto MyPlayerState = (AWFBasePlayerState*)AcknowledgedPawn->PlayerState;
	auto MyTeamId = MyPlayerState->AttributeTeamID;

	int32_t sx, sy;
	PlayerController->GetViewportSize(sx, sy);
	winCenter={ sx / 2.0f,sy / 2.0f };
	if (bShowRange)
		ImGui::GetForegroundDrawList()->AddCircle({ winCenter.X,winCenter.Y }, AimRange, ImColor(1.F, 1.F, 1.F, 1.F), 100);
		//ZeroGUI::DrawCircle(winCenter, FOV, 100, { 1.0,1.0,1.0,1.0 });

	auto GamePlay = (UGameplayStatics*)UGameplayStatics::StaticClass();
	TArray<AActor*> Actors;
	GamePlay->STATIC_GetAllActorsOfClass(World, AWFBaseCharacter::StaticClass(), Actors);

	for (int32_t i = 0; i < Actors.Count(); i++) {
		if (!Actors[i] || Actors[i] == AcknowledgedPawn)continue;

		auto Player = (AWFBaseCharacter*)Actors[i];
		if (!Player->PlayerState)continue;
		auto PlayerState = (AWFBasePlayerState*)Player->PlayerState;
		auto Health = PlayerState->CyAttributeSet->Health.CurrentValue;
		auto Armor = PlayerState->CyAttributeSet->Armor.CurrentValue;

		if (Health <= 0 || PlayerState->AttributeTeamID == MyTeamId)continue;

		FVector Origin, Extend;
		Player->GetActorBounds(true, Origin, Extend, false);

		FVector2D footPos;
		if (!PlayerController->ProjectWorldLocationToScreen({ Origin.X, Origin.Y, Origin.Z + (Extend.Z / 1.5f) }, footPos, 0)) continue;
		FVector2D headPos;
		if (!PlayerController->ProjectWorldLocationToScreen({ Origin.X, Origin.Y, Origin.Z - (Extend.Z / 1.5f) }, headPos, 0)) continue;
		const float height = abs(footPos.Y - headPos.Y);
		const float width = height * 0.5f;
		FVector2D top = { headPos.X - width * 0.5f, headPos.Y + 5.0f };
		FVector2D bottom = { headPos.X + width * 0.5f, footPos.Y };

		if(bBox)
			DrawBox(top, bottom, ImColor(1.f,1.f,1.f,1.f));

		if (bBone)
			Bones::Draw(PlayerController, Player->Mesh);

		if (bDrawLine) {
			DrawLine({ winCenter.X,  sy * 1.0f }, headPos, 1.0f, BoxColor);
		}

		float Add = 15.f;

		if (bHealth) {
			char cHealth[0x20];
			sprintf(cHealth, "Hp:%0.f Armor%0.f", Health, Armor);
			ImGui::GetForegroundDrawList()->
				AddText({ headPos.X ,headPos.Y + Add }, ImColor(0.f, 1.f, 0.f, 1.f), cHealth);
			Add += 15.f;
		}

		if (bDrawName) {
			ImGui::GetForegroundDrawList()->
				AddText({ headPos.X ,headPos.Y + Add }, ImColor(0.f, 1.f, 0.f, 1.f), PlayerState->PlayerNamePrivate.ToString().c_str());
			Add += 15.f;
		}

		auto distance = winCenter.Distance(headPos) / 100.F;

		if (bDistance) {
			char cDis[0x15];
			sprintf(cDis, "%0.f M", AcknowledgedPawn->K2_GetActorLocation().Distance(Origin) / 100.f);
			ImGui::GetForegroundDrawList()->
				AddText({ headPos.X ,headPos.Y + Add }, ImColor(0.f, 1.f, 0.f, 1.f), cDis);
		}
		
		if (bVisible) {
			auto SrcPos = PlayerController->PlayerCameraManager->GetCameraLocation();
			auto TarPos = Player->Mesh->GetBoneWorldPos(Player->Mesh->GetBoneIndex(Aimto));
			if (Trace(Actors, SrcPos, TarPos))
				continue;
		}

		if (distance < dis && distance < AimRange) {
			dis = distance;
			Target = Player;
		}
	}

	if (bAimBot && GetAsyncKeyState(aimKey)) {
		if (Target != nullptr) {
			auto TarPos = Target->Mesh->GetBoneWorldPos(Target->Mesh->GetBoneIndex(Aimto));
			FVector2D TarSreen;
			PlayerController->ProjectWorldLocationToScreen(TarPos, TarSreen, false);

			FRotator angle = ((TarPos - PlayerController->PlayerCameraManager->GetCameraLocation()) /_smess).ToRotator();
			PlayerController->SetControlRotation(angle);
		}
	}

}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			DefaultTheme();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Luna Ins[show/hide]",0,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize("Luna Ins[show/hide]", ImVec2(300, 480));
	ImGui::Spacing();
	ImGui::Text("Visuals");
	ImGui::Checkbox("Box", &bBox);
	ImGui::Checkbox("Bone", &bBone);
	ImGui::Checkbox("Line", &bDrawLine);
	ImGui::Checkbox("Health", &bHealth);
	ImGui::Checkbox("Name", &bDrawName);
	ImGui::Checkbox("Distance", &bDistance);
	ImGui::Spacing();
	ImGui::Text("AimBot");
	ImGui::Checkbox("Enable AimBot", &bAimBot);
	ImGui::Checkbox("Show Range", &bShowRange);
	ImGui::Checkbox("Barrier Check", &bVisible);
	ImGui::Text("AimKey:");
	ImGui::SameLine();
	HotkeyButton(aimKey, "", "##29r9102", satg);
	ImGui::Text("AimPos:");
	ImGui::SameLine();
	if (ImGui::Combo("", &style_idx, "Head\0Neck\0Pelvis\0"))
	{
		switch (style_idx)
		{
		case 0: Aimto = Bones::Head; break;
		case 1: Aimto = Bones::Neck; break;
		case 2: Aimto = Bones::Pelvis; break;
		}
	}

	ImGui::SliderFloat("Smoothness", &_smess, 1.f, 10.f, "%0.f", 1.f);
	ImGui::Text(" Range:");
	ImGui::SameLine();
	ImGui::SliderFloat("", &AimRange, 40.f, 300.f, "%0.f", 20.f);
	ImGui::End();
	Tick();
	ImGui::Render();


	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	console_Initialize();
	Sleep(15000);
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);
			InitSdk();
			Bones::Init();
			init_hook = true;
		}
	} while (!init_hook);
	
/*	while (true) {
		if (GetAsyncKeyState(VK_F6)){
			console_Shutdown();
			kiero::shutdown();
			FreeLibraryAndExitThread((HMODULE)lpReserved, 0);
		}
	}*/
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}