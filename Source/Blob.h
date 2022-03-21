#pragma once
#include <vector>
#include <cstdint>
class BinaryBlob {
public:
	BinaryBlob() {}
	BinaryBlob(const BinaryBlob&) = default;
	BinaryBlob(BinaryBlob&&) noexcept = default;

	BinaryBlob& operator=(const BinaryBlob&) = default;
	BinaryBlob& operator=(BinaryBlob&&) noexcept = default;

	void LoadFromFile(const wchar_t* filePath);
	bool IsEmpty() const noexcept;
	bool IsUTF8() const noexcept;
private:
	std::vector<uint8_t> m_binaryData;
};