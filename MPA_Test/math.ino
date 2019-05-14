int winkelDifferenz(int x, int y) {
  int z;
  z = abs(x-y);
  if (z > 180) z = 360 - z;
  return z;
  
}

