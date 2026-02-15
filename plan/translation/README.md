# RuffleCpp Translation Plan

This directory contains the plan for translating the Ruffle Rust codebase to C++ with wxWidgets.

## Track: Translation

### Phase 1: Dummy Translation
- Directory: `dummy-translation/`
- Objective: Translate every Rust file directly to C++ without checking anything initially
- Each Rust file has its own subdirectory with a task.md file describing the translation task
- Total: 669 Rust files translated one-by-one

### Phase 2: Build & Error Correction
- Directory: `build-errors/`
- Objective: Build C++ files and check for compilation errors
- Fix header inclusions and dependencies
- Create necessary header files as needed during translation

### Phase 3: Non-Rust Files
- Directory: `non-rust-files/`
- Objective: Handle all other files (likely remain the same but check for Rust-specific elements)
- Update build configurations, documentation, etc.

### Phase 4: Executable Verification
- Directory: `executables/`
- Objective: Ensure all executables work correctly
- Test functionality end-to-end

## Technology Stack
- C++ with wxWidgets (instead of Qt) for GUI components
- Standard C++ libraries for other functionality
- Maintaining original Ruffle functionality during translation

## Progress Tracking
Each task has a status checklist in its respective task.md file to track completion.