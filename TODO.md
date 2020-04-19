# TODO

## platform

* convert from game pixel color format for platform. don't assume it matches

## game

* learn something/anything about how to write a decent 2D renderer
    + would drawing entity-by-entity rather than pixel-by-pixel be faster?
    + why? better branch prediction?
    + current rendering can easily parallelize though. it's essentially a giant shader,
      but currently running single-threaded.