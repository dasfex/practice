auto getHexStringFromNumber = [](auto number) {
  std::stringstream ss;
  for (size_t i = 0, size = sizeof(number) * 2; i < size; ++i) {
    ss << std::hex << ((number >> (i * 4)) & 0xF);
  }
  return ss.str();
};
