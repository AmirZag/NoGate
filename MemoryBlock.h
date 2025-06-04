#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

#include <QByteArray>
#include <bitset>


namespace Afc
{
template<size_t BlockSize>
class MemoryBlock
{
  static constexpr size_t  BITS_PER_BYTE = 8;
  static constexpr size_t  BITS_COUNT    = BITS_PER_BYTE * BlockSize;

public:
  MemoryBlock()
  {
  }

  MemoryBlock(const QByteArray &bytes)
  {
    if (bytes.size() != BlockSize)
    {
      throw "Invalid data size for MemoryBlock";
    }

    for (size_t i = 0; i < BlockSize; ++i)
    {
      for (size_t b = 0; b < BITS_PER_BYTE; ++b)
      {
        _bits[i * BITS_PER_BYTE + b] = bytes.at(BlockSize - 1 - i) & (1 << b);
      }
    }
  }

  QByteArray  getData() const
  {
    QByteArray  bytes;

    bytes.resize((BITS_COUNT + BITS_PER_BYTE - 1) / BITS_PER_BYTE);
    bytes.fill(0);

    for (size_t b = 0; b < BITS_COUNT; ++b)
    {
      const int  offset = b / BITS_PER_BYTE;
      bytes[offset] = bytes.at(offset) | ((_bits[BITS_COUNT - 1 - b] ? 1 : 0) << ((BITS_PER_BYTE - 1 - b) % BITS_PER_BYTE));
    }

    return bytes;
  }

  template<std::size_t Offset = 0, std::size_t Length = BITS_COUNT - Offset>
  std::bitset<Length>  readBits() const
  {
    static_assert(Offset + Length <= BITS_COUNT, "Invalid bit range.");

    std::bitset<Length>  res;

    for (size_t i = 0; i < Length; ++i)
    {
      res[Length - 1 - i] = _bits[BITS_COUNT - Offset - 1 - i];
    }

    return res;
  }

  template<std::size_t Offset = 0, std::size_t Length>
  void  writeBits(const std::bitset<Length> &bits)
  {
    static_assert(Offset + Length <= BITS_COUNT, "Invalid bit range.");

    for (size_t i = 0; i < Length; ++i)
    {
      _bits[BITS_COUNT - Offset - 1 - i] = bits[Length - 1 - i];
    }
  }

private:
  std::bitset<BITS_COUNT>  _bits;
};
}

#endif // MEMORYBLOCK_H
