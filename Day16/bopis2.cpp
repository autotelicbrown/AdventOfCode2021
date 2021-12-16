#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>


enum ID_type : unsigned long long int
{
  ID_SUM = 0,
  ID_PRODUCT = 1,
  ID_MIN = 2,
  ID_MAX = 3,
  ID_LITERAL = 4,
  ID_GREATER = 5,
  ID_LESSER = 6,
  ID_EQUAL = 7
};

class HexReader {
  public:
    HexReader() : buffer(0), bitsBuffered(0) {}

    unsigned long long int Read(unsigned long long int nBits)
    {
      unsigned long long int val = 0;
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
    unsigned long long int buffer;
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

    unsigned long long int DecodePacket(unsigned long long int &outVal) {
      unsigned long long int version = hexReader.Read(3);
      ID_type id = static_cast<ID_type>(hexReader.Read(3));
      unsigned long long int bitsParsed = 6;
      switch (id) {
        case ID_LITERAL:
          bitsParsed += DecodeLiteral(outVal);
          break;
        default:
          bitsParsed += DecodeOperator(id, outVal);
          break;
      }
      return bitsParsed;
    }

  private:
    HexReader hexReader;
    unsigned long long int versionNumberSum;

    unsigned long long int DecodeLiteral(unsigned long long int &outVal) {
       bool lastGroup = false;
       outVal = 0;
       unsigned long long int bitsParsed = 0;
       
       do {
         if (hexReader.Read(1) == 0)
           lastGroup = true;

         outVal = (outVal << 4);
         outVal += hexReader.Read(4);
         bitsParsed += 5;
       } while (!lastGroup);

       return bitsParsed;
    }

    unsigned long long int DecodeOperator(ID_type id, unsigned long long int &outVal) {
      unsigned long long int lengthType = hexReader.Read(1);
      unsigned long long int bitsParsed = 1;
      std::vector<unsigned long long int> buf;

      if (lengthType == 0) {
        unsigned long long int bitLength = hexReader.Read(15);
        bitsParsed += 15 + bitLength;
        while (bitLength > 0) {
          buf.push_back(0);
          bitLength -= this->DecodePacket(buf.back());
        }
      } else {
        unsigned long long int nPackets = hexReader.Read(11);
        bitsParsed += 11;
        for (unsigned long long int i = 0; i < nPackets; ++i) {
          buf.push_back(0);
          bitsParsed += this->DecodePacket(buf.back());
        }
      }

      switch (id) {
        case ID_SUM:
          outVal = 0;
          for (unsigned long long int val: buf)
            outVal += val;
          break;
        case ID_PRODUCT:
          outVal = 1;
          for (unsigned long long int val: buf)
            outVal *= val;
          break;
        case ID_MIN:
          outVal = buf[0];
          for (unsigned long long int val: buf)
            outVal = std::min(outVal, val);
          break;
        case ID_MAX:
          outVal = buf[0];
          for (unsigned long long int val: buf)
            outVal = std::max(outVal, val);
          break;
        case ID_GREATER:
          outVal = (buf[0] > buf[1]) ? 1 : 0;
          break;
        case ID_LESSER:
          outVal = (buf[0] < buf[1]) ? 1 : 0;
          break;
        case ID_EQUAL:
          outVal = (buf[0] == buf[1]) ? 1 : 0;
          break;
        default:
          printf("Invalid operator id %llu\n", static_cast<unsigned long long>(id));
          std::exit(-1);
      }

      return bitsParsed;
    }
};

int main(int argc, char *argv[])
{
  Decoder decoder;

  unsigned long long int ans;
  (void) decoder.DecodePacket(ans);

  printf("Answer: %llu\n", ans);

  return 0;
}
