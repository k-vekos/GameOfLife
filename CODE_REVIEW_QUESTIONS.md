* I forgot to include `<cstdint>` but it still compiled okay?
* I see you removed `return 0;` from `main()`, could you explain why?
* About `auto const& getLivingCells() const { return aliveCells; }`
  * I know the `const` after the function signature means this function cannot cause any side-effects, but what does the `const&` before it mean in this context?
  * I had `std::uint8_t & getCell(int x, int y);` in my original code, is it more typical to not put a space before the `&`?

* "Here, I've used `emplace_back` to reduce the likelihood of copying" -- could you explain this? Is it different than `push_back` in that way?
* In `GameOfLife.cpp` is what you've done with the `neighbours` array going to actually be more performant than what I had before? Otherwise I understand it's probably more readable than what I was doing.
* What does `omp critical` do here? Is `aliveCells` only accessible here in `critical`?
* I see in `WorldRenderer.h` you added `= {};` to `m_vertexPoints`, is this just for clarity to the reader?
* I see you named `next_generation` using Snake Case, is that more typical for variables in C++?
* In `WorldRenderer.renderBackgrounds()` your modified code has `topLeft` and `bottomRight` initialized to some value but it looks like it's never used without being overwritten first? Actually it looks like you reuse the names `topLeft` and `bottomRight` inside the subsequent for loop, so I guess this is just a typo?