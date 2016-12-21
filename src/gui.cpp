#include "gui.h"

#ifdef _WIN32

#include <windows.h>

char *filename_popup(const char *filter) {
	static char filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags |= OFN_NOCHANGEDIR;

	if (GetOpenFileName(&ofn)) {
		return filename;
	} else {
		return nullptr;
	}
}

void message_box_error(const char *message) {
	MessageBox(NULL, message, "Error", MB_OK | MB_ICONERROR);
}

#else

#include <cstdio>

char *filename_popup(const char *filter) {
	message_box_error("I have no idea how to open a OpenFileName dialog in linux.");
	return nullptr;
}

void message_box_error(const char *message) {
	fprintf(stderr, "%s\n", message);
}

#endif