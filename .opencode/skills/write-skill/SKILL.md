---
name: Write a skill
description: Create or update an OpenCode skill for better-gscemu; use when adding SKILL.md files, frontmatter, supporting references or scripts, and checking skill discovery.
---

# Write a skill

Project skills live in `.opencode/skills/<id>/SKILL.md` and are committed. OpenCode discovers `.opencode/skills`, `.claude/skills`, and `.agents/skills`; `.skills` is not a default source unless it is added to the `skills` array in OpenCode config.

## File shape

```text
.opencode/skills/
└── verify-c/
    ├── SKILL.md
    ├── references/
    │   └── checks.md
    └── scripts/
        └── verify.sh
```

`SKILL.md`:

```markdown
---
name: Verify C changes
description: Verify better-gscemu C changes; use before finishing to run a clean build, smoke tests, analyzers, and a short fuzz run.
---

# Verify C changes

Run the script, then report which checks ran.

## Rules

- Do not suppress warnings.
- If clang is missing, say fuzzing was skipped.
```

## Rules

- The skill ID comes from the directory name, not the frontmatter `name`. Use lowercase kebab-case.
- `description` is what makes the skill discoverable. Include the repo and the task trigger.
- Keep the body short and actionable. Put long procedures in `references/`; put repeatable commands in `scripts/`.
- Support file paths are relative to the directory containing `SKILL.md`.
- Use `metadata.opencode/autoinvoke: false` or `disable-model-invocation: true` when the skill should only be loaded explicitly.
- Do not duplicate `AGENTS.md`. Point at it and add only the task-specific workflow.
- Scripts must be executable, non-destructive, and run from the repo root. Use `git rev-parse --show-toplevel` instead of fragile relative paths.
- Do not add network access, `sudo`, or destructive commands unless the user explicitly asks.
- Check the skill appears in the available list after saving, and run any script before claiming it works.

See [template](references/template.md) for a copy-paste starting point.
