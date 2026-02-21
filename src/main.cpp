#include <cstdio>
#include <cstring>
#include <cstdint>
#include <stdlib.h>

class scale
{
 public:
  inline scale(unsigned sequenceLen, unsigned notesPerScale, unsigned maxInterval, unsigned maxCluster)
      : m_sequenceLen { sequenceLen }
      , m_notesPerScale { notesPerScale }
      , m_maxInterval { maxInterval }
      , m_maxCluster { maxCluster }
  {
    printf("sequence-length : %d\n", m_sequenceLen);
    printf("notes-per-scale : %d\n", m_notesPerScale);
    printf("max-interval    : %d\n", m_maxInterval);
    printf("max-cluster     : %d\n", m_maxCluster);
  }

  inline void generateScales()
  {
    puts("searching scales...\n");
    unsigned const numberOfPatterns = 1 << (m_sequenceLen - 1);

    unsigned baseScales  = 0u;
    unsigned totalScales = 0u;
    for (uint64_t bitPattern = 0b0; bitPattern < numberOfPatterns; bitPattern++)
    {
      auto const startPattern = (bitPattern << 1) | 0b1;
      if (!checkPattern(startPattern))
        continue;
      ++totalScales;
      printf("#%02d: ", ++baseScales);
      printScale(startPattern);
      for (auto pattern = findNextMode(startPattern); pattern != startPattern; pattern = findNextMode(pattern))
      {
        totalScales++;
        printf("     ");
        printScale(pattern);
      }
      printf("\n");
    }
    printf("%d base scales found, %d total scales\n", baseScales, totalScales);
  }

 private:
  unsigned m_sequenceLen;
  unsigned m_notesPerScale;
  unsigned m_maxInterval;
  unsigned m_maxCluster;

  inline void printScale(uint64_t const pattern) const
  {
    printScaleAsPattern(pattern);
    printf("  ");
    printScaleAsIntervals(pattern);
    printf("\n");
  }

  inline void printScaleAsPattern(uint64_t pattern) const
  {
    for (auto shift = 0u; shift < m_sequenceLen; shift++)
      (pattern & 0b1) ? putchar('x') : putchar('-'), pattern >>= 1;
  }

  inline void printScaleAsIntervals(uint64_t pattern) const
  {
    unsigned interval = 1u;
    pattern |= ((pattern & 0b1) << m_sequenceLen);
    for (auto shift = 0u; shift < m_sequenceLen; shift++)
    {
      pattern >>= 1;
      if (pattern & 0b1)
      {
        if (shift < m_sequenceLen - 1)
          printf("%d-", interval);
        else
          printf("%d", interval);
        interval = 1u;
      }
      else
        interval++;
    }
  }

  inline uint64_t rotateRight(uint64_t const pattern) const
  {
    auto const mask      = (uint64_t(1) << m_sequenceLen) - 1;
    auto const upperBits = pattern >> 1;
    auto const lowerBits = pattern << (m_sequenceLen - 1);
    return mask & (upperBits | lowerBits);
  }

  inline bool checkPattern(uint64_t const patternToCheck) const
  {
    auto numberOfNotesInPattern = 0u;
    auto maxInterval            = 1u;
    auto currentInterval        = 1u;
    auto maxCluster             = 0u;
    auto currentCluster         = 0u;
    auto pattern                = patternToCheck;
    for (auto notePos = 0u; notePos < m_sequenceLen; notePos++)
    {
      bool haveNote = (pattern & 0b1) != 0u;

      if (haveNote && (++numberOfNotesInPattern > m_notesPerScale))
        return false;

      if (!haveNote && (++currentInterval > maxInterval))
        if ((maxInterval = currentInterval) > m_maxInterval)
          return false;
      if (haveNote)
        currentInterval = 1u;

      if (haveNote && (++currentCluster > maxCluster))
        if ((maxCluster = currentCluster) > m_maxCluster)
          return false;
      if (!haveNote)
        currentCluster = 0u;

      pattern  = rotateRight(pattern);
      haveNote = (pattern & 0b1) != 0u;
      if (haveNote && (pattern < patternToCheck))  // smaller valued pattern means this one is a mode, not base
        return false;
    }
    if (numberOfNotesInPattern != m_notesPerScale)
      return false;
    return true;
  }

  inline uint64_t findNextMode(uint64_t pattern) const
  {
    for (auto notePos = 0u; notePos < m_sequenceLen; notePos++)
    {
      pattern = rotateRight(pattern);
      if (pattern & 0b1)
        break;
    }
    return pattern;
  }
};

inline void usage()
{
  puts("Usage: find-scales <sequence-len> <notes-per-scale> <max-interval> <max-cluster>");
}

inline bool scanParam(char const *const buffer, unsigned &param, unsigned min, unsigned max, char const *const text)
{
  if (!buffer || (1 != sscanf(buffer, "%d", &param)))
    return printf("Error: cannot convert parameter '%s'\n", text), false;
  if (param < min || param > max)
    return printf("Error: parameter '%s' not in range %d...%d\n", text, min, max), false;
  return true;
}

int main(int argc, char *argv[])
{
  if (argc != 5)
    return puts("Error: 4 parameters must be given"), usage(), -1;

  unsigned sequenceLen;
  if (!scanParam(argv[1], sequenceLen, 2, 63, "sequence-len"))
    return usage(), -1;

  unsigned notesPerScale;
  if (!scanParam(argv[2], notesPerScale, 2, sequenceLen, "notes-per-scale"))
    return usage(), -1;

  unsigned maxInterval;
  if (!scanParam(argv[3], maxInterval, 1, sequenceLen, "max-interval"))
    return usage(), -1;

  unsigned maxCluster;
  if (!scanParam(argv[4], maxCluster, 1, sequenceLen, "max-cluster"))
    return usage(), -1;

  class scale scale
  {
    sequenceLen, notesPerScale, maxInterval, maxCluster
  };

  scale.generateScales();

  return 0;
}
