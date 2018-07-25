# Hex Dump

Mock file calls and dump binary files to console for target platforms
without a file system. Dumping is handled specially to be faster, though
the terminal also matters. Tera Term is reasonably fast and has a large
buffer.

Recommended dump size is around 16 MB or less, otherwise it starts to
take a while.

### Usage

Include the header and define `REPLACE_CALLS` to mock out file calls
like `fwrite` and `fopen`. Modify `LOG_FILE_SIZE` to your liking. Then
make sure the proper symbols are linked, or include the source as well.

Also ships with `convert` which translates the dump back into a binary file.
