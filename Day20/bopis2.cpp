#include <cstdio>
#include <vector>
#include <algorithm>
#include <cassert>

const static int INITIAL_DIM = 100;
const static int TABLE_SIZE = 512;
const static int NUM_ENHANCEMENT = 50;

class Image {
  public:
    Image(int dim) : dim(dim), val(dim * dim, 0) {}

    void ReadImageValuesFromInput() {
      char c;
      for (auto i = 0; i < val.size(); ++i) {
        while (1) {
          c = getchar();
          if (c == EOF) {
            printf("Missing image values in input\n");
            std::exit(-1);
          }
          if (c == '#' || c == '.')
            break;
        }
        val[i] = (c == '#' ? 1 : 0);
      }
    }

    void WoozyConvolution(int *lookupTable, int outVal) {
      this->AddPixelBorder(outVal);
      std::vector<int> res(val.size());

      int adjx[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
      int adjy[9] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};

      for (int y = 0; y < dim; ++y) {
        for (int x = 0; x < dim; ++x) {
          int idx = x + y * dim;
          int tmp = 0;
          for (int i = 0; i < 9; ++i) {
            tmp <<= 1;
            int dx = x + adjx[i];
            int dy = y + adjy[i];
            if (dx < 0 || dx >= dim || dy < 0 || dy >= dim)
              tmp += outVal;
            else
              tmp += val[dx + dy * dim];
          }
          assert(tmp < TABLE_SIZE);
          res[idx] = lookupTable[tmp];
        }
      }

      std::swap(res, val);
    }

    int CountBrightPixels() {
      int res = 0;
      for (int i: val)
        res += i;
      return res;
    }

    void PrintImage() {
      printf("\n");
      for (int y = 0; y < dim; ++y) {
        for (int x = 0; x < dim; ++x) {
          int idx = x + y * dim;
          printf("%c", (val[idx] > 0 ? '#' : '.'));
        }
        printf("\n");
      }
    }

  private:
    int dim;
    std::vector<int> val;

    void AddPixelBorder(int borderVal) {
      int oldDim = dim;
      dim += 2;
      val.resize(dim * dim, borderVal);
      // Reverse iteration to work in-place
      for (int y = oldDim - 1; y >= 0; --y) {
        for (int x = oldDim - 1; x >= 0; --x) {
          int oldIdx = x + y * oldDim;
          int newIdx = (x + 1) + (y + 1) * dim;
          std::swap(val[oldIdx], val[newIdx]);
        }
      }
    }
};

int main(int argc, char *argv[]) {
  int lookupTable[TABLE_SIZE] = {0}; 

  for (int i = 0; i < TABLE_SIZE; ++i)
    lookupTable[i] = (getchar() == '#' ? 1 : 0);

  Image img(INITIAL_DIM);
  img.ReadImageValuesFromInput();

  int outsideVal = 0;
  for (int i = 0; i < NUM_ENHANCEMENT; ++i) {
    img.WoozyConvolution(lookupTable, outsideVal);
    outsideVal = lookupTable[(outsideVal == 0 ? 0 : 511)];
  }

  img.PrintImage();
  int ans = img.CountBrightPixels();
  printf("Answer = %d\n", ans);

  return 0;
}
