# TODO

## platform

* convert from game pixel color format for platform. don't assume it matches
* repainting seems to stop when moved to another monitor, although it doesn't
  crash like I might have expected if the surface was destroyed and a new one
  created
* vsync

## game

* learn something/anything about how to write a decent 2D renderer
    + would drawing entity-by-entity rather than pixel-by-pixel be faster?
    + why? better branch prediction?
    + current rendering can easily parallelize though. it's essentially a giant shader,
      but currently running single-threaded.
    + on second thought, maybe I'm not using the SDL window/surface funcs optimally, because
      even drawing almost nothing I don't think I even get 200 fps, shouldn't I be able to just
      blit a color like 1000 times per second?