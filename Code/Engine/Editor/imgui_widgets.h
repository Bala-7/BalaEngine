#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include <string>

namespace nimgui
{
	static void draw_vec3_widget(const char* label, glm::vec3& values, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];


		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushFont(boldFont);
		ImGui::Button("X", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		char floatFieldNameX[100] = "##";
		strcat(floatFieldNameX, label);
		strcat(floatFieldNameX, "X");
		ImGui::DragFloat(floatFieldNameX, &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::Button("Y", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		char floatFieldNameY[100] = "##";
		strcat(floatFieldNameY, label);
		strcat(floatFieldNameY, "Y");
		ImGui::DragFloat(floatFieldNameY, &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::Button("Z", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		char floatFieldNameZ[100] = "##";
		strcat(floatFieldNameZ, label);
		strcat(floatFieldNameZ, "Z");
		ImGui::DragFloat(floatFieldNameZ, &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
	}
}
