# PngStream

A generalized streamy way to render out png's

The output is a string that can be converted to a binary `uint8_t` vector and 
written to a file stream that would be seen
Or you can also use the xxd utility to check it out
```bash
make run | xxd
```

The output would be something like this:

![Output example](assets/screenshot_updated.png)

Licensed under AGPL

Libs used: libpng
