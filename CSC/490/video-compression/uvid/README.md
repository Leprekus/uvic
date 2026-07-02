# For Me
## Compress

Entities at work :
- reader
- stream

**reader**
Handles stream ingestion.

**stream**
Handles processed stream emission.


Things to keep in mind:
- **Keep your data structures tight.** Pass your pixel blocks or residuals 
by reference or use lightweight views/spans to avoid copying memory between 
these files.
- If a stage doesn't need to remember state, it shouldn't be a class. You can 
just use a Namespace with free-standing functions instead of a class.
