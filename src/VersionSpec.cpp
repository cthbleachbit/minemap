//
// Created by cth451 on 2020/05/15.
//

#include "VersionSpec.h"
#include "constants.h"

#include <iostream>

#ifdef WIN32
#include <libloaderapi.h>
#include <windows.h>
#include <WinError.h>
#else
#include <locale>
#include <codecvt>
#include <unistd.h>
#endif

namespace Minemap {


	/**
	 * @return the directory where the program is located or nullopt if this is unavailable
	 */
	static std::optional<std::wstring> getProgramPath() {
#ifdef WIN32
		wchar_t buf[2048];
		SetLastError(NO_ERROR);
		int ret = GetModuleFileName(nullptr, buf, 2048);
		int error_code = GetLastError();
		std::cerr << "Warning: Cannot determine executable location" << std::endl;
		return (error_code != NO_ERROR) ? std::nullopt : std::make_optional(std::wstring(buf));
#else
		// Apparently this doesn't work on Mac OS but I don't give shet
		char buf[2048];
		int ret = readlink("/proc/self/exe", buf, 2048);
		if (ret < 0 || ret == 2048) {
			return std::nullopt;
		}
		buf[ret] = 0;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.from_bytes(buf);
#endif
	}


	/**
	 * Returns absolute path to a valid palette
	 * @param ver   Version number
	 * @return
	 */
	std::filesystem::path verSpecToPalettePath(Version ver) {
		if (ver < 0 || ver >= END_OF_VERSION) {
			throw std::runtime_error(INVALID_GAME_VER);
		}

		auto programDir = getProgramPath();
		if (programDir.has_value()) {
			std::filesystem::path p = absolute(std::filesystem::path(programDir.value()));
			if (std::filesystem::is_regular_file(p / SUPPORTED_VERSIONS[ver].palettePath)) {
				return p.parent_path() / SUPPORTED_VERSIONS[ver].palettePath;
			}
		}

#if WIN32
		// Windows ONLY: prefer palettes next to the program executable, or fall back to cwd
		return std::filesystem::current_path() / SUPPORTED_VERSIONS[ver].palettePath;
#else
		// Unix ONLY: Find palette in the palette directory
		return std::filesystem::path(MINEMAP_PALETTE_DIR) / SUPPORTED_VERSIONS[ver].palettePath;
#endif
	}

	void insertDataVersion(NBTP::CompoundTag &root, Version ver) {
		if (ver < 0 || ver >= END_OF_VERSION) {
			throw std::runtime_error(INVALID_GAME_VER);
		}
		auto dv = SUPPORTED_VERSIONS[ver].dataVersion;
		if (dv.has_value()) {
			root.insert("DataVersion", std::make_shared<NBTP::IntTag>(dv.value()));
		}
	}

	Version verSpecFromString(const std::string &verString) noexcept {
		auto comparePredicate = [&verString](const VersionSpec &spec) { return spec.name == verString; };
		auto itr = std::find_if(SUPPORTED_VERSIONS.cbegin(), SUPPORTED_VERSIONS.cend(), comparePredicate);
		if (itr == SUPPORTED_VERSIONS.cend()) {
			return Version::INVALID;
		} else {
			return static_cast<Version>(itr - SUPPORTED_VERSIONS.cbegin());
		}
	}
}