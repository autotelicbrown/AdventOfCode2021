#include <cstdio>
#include <cstdlib>

static const int ID_LITERAL = 4;

class HexReader {
  public:
    HexReader() : buffer(0), bitsBuffered(0) {}

    unsigned int Read(unsigned int nBits)
    {
      unsigned int val = 0;
      for (int i = 0; i < nBits; ++i) {
        if (!bitsBuffered && !this->ReadHexInput())
          return val;

        val = (val << 1);
        int offset = bitsBuffered - 1;
        val += (buffer >> offset);
        buffer -= ((buffer >> offset) << bitsBuffered - 1) ; 
        bitsBuffered -= 1;
      }

      return val;
    }

  private:
    unsigned int buffer;
    int bitsBuffered;

    bool ReadHexInput() {
      char c;
      if (scanf("%c", &c) == EOF || c == '\n')
        return false;

      if (c >= '0' && c <= '9')
        this->buffer = c - '0';
      else if (c >= 'A' && c <= 'F')
        this->buffer = 10 + (c - 'A');
      else {
        printf("Invalid character %c in input stream\n", c);
        std::exit(-1);
      }

      this->bitsBuffered = 4;

      return true;
    }
};

class Decoder {
  public:
    Decoder() : hexReader(), versionNumberSum(0) {}

    unsigned int DecodePacket() {
      unsigned int version = hexReader.Read(3);
      unsigned int id = hexReader.Read(3);
      unsigned int bitsParsed = 6;
      switch (id) {
        case ID_LITERAL:
          bitsParsed += DecodeLiteral();
          break;
        default:
          bitsParsed += DecodeOperator();
          break;
      }
      versionNumberSum += version;
      return bitsParsed;
    }

    unsigned int GetVersionNumberSum() {
      return versionNumberSum;
    }

  private:
    HexReader hexReader;
    unsigned int versionNumberSum;

    unsigned int DecodeLiteral() {
       bool lastGroup = false;
       unsigned int val = 0;
       unsigned int bitsParsed = 0;
       
       do {
         if (hexReader.Read(1) == 0)
           lastGroup = true;

         val = (val << 4);
         val += hexReader.Read(4);
         bitsParsed += 5;
       } while (!lastGroup);

       return bitsParsed;
    }

    unsigned int DecodeOperator() {
      unsigned int lengthType = hexReader.Read(1);
      unsigned int bitsParsed = 1;

      if (lengthType == 0) {
        unsigned int bitLength = hexReader.Read(15);
        bitsParsed += 15 + bitLength;
        while (bitLength > 0)
          bitLength -= this->DecodePacket();
      } else {
        unsigned int nPackets = hexReader.Read(11);
        bitsParsed += 11;
        for (unsigned int i = 0; i < nPackets; ++i)
          bitsParsed += this->DecodePacket();
      }

      return bitsParsed;
    }
};

int main(int argc, char *argv[])
{
  Decoder decoder;

  unsigned int bitsParsed = decoder.DecodePacket();

  unsigned int ans = decoder.GetVersionNumberSum(); 
  printf("Answer: %u\n", ans);

  return 0;
}
