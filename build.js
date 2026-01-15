import fs from "fs";
import path from "path";
import crypto from "crypto";
import { execSync } from "child_process";

const cachePath = "./build/cache/chess.cache";
const srcPath = "./src";
const appPath = "./build/chess.exe";

const getFileHash = (filePath) => {
  const fileBuffer = fs.readFileSync(filePath);
  const hashSum = crypto.createHash("sha256");
  hashSum.update(fileBuffer);
  return hashSum.digest("hex");
};

const findAllfiles = (currentPath) => {
  let results = [];

  const files = fs.readdirSync(currentPath, { withFileTypes: true });

  for (const file of files) {
    const fullPath = path.join(currentPath, file.name);
    if (file.isDirectory()) {
      results = results.concat(findAllfiles(fullPath));
    } else if (
      file.isFile() &&
      (file.name.endsWith(".cpp") || file.name.endsWith(".h"))
    ) {
      const hash = getFileHash(fullPath);
      results.push({ path: fullPath, hash });
    }
  }

  return results;
};

const files = findAllfiles(srcPath);
let prevCache = [];

fs.mkdirSync(path.dirname(cachePath), { recursive: true });

if (fs.existsSync(cachePath)) {
  prevCache = JSON.parse(fs.readFileSync(cachePath, "utf-8"));
}

const changed = [];
files.forEach((file) => {
  const prev = prevCache.find((f) => f.path === file.path);
  if (!prev) {
    changed.push(file);
    return;
  }

  if (prev.hash !== file.hash) changed.push(file);
});

let filesToCompile = changed.map((file) => file.path);

if (!fs.existsSync(appPath)) {
  filesToCompile = files.map((file) => file.path);
}

filesToCompile = filesToCompile.filter((filePath) => !filePath.endsWith(".h"));

if (filesToCompile.length != 0) {
  execSync(
    `cl.exe /Zi /Od /EHsc /nologo /I"lib/include" /std:c++17 /Fobuild/objects/ /Fdbuild/compiler.pdb ${filesToCompile.join(
      " "
    )} /c `,
    {
      stdio: "inherit",
      shell: true,
    }
  );

  execSync(
    `link.exe /OUT:"build/chess.exe" /DEBUG /PDB:"build/chess.pdb" build/objects/*.obj SDL3.lib SDL3_image.lib User32.lib /LIBPATH:"lib/SDL3" /SUBSYSTEM:WINDOWS`,
    {
      stdio: "inherit",
      shell: true,
    }
  );
}

fs.writeFileSync(cachePath, JSON.stringify(files));
