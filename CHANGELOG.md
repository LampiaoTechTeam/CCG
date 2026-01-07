## 9.4.0 (2026-01-07)

* feat: removido variaveis globais dos mosntros no save do jogo para variaveis por parametro ([b711c55](https://github.com/repiazza/CCG/commit/b711c55))

## <small>9.3.1 (2026-01-07)</small>

* fix: Renderizacao das imagens no linux ([186261e](https://github.com/repiazza/CCG/commit/186261e))

## 9.3.0 (2026-01-06)

* ci: added needed commit-analyzer package for semver ([486292b](https://github.com/repiazza/CCG/commit/486292b))
* ci: added needed git package for semver ([abaa64d](https://github.com/repiazza/CCG/commit/abaa64d))
* ci: added needed package changelog to semver ([9af9a21](https://github.com/repiazza/CCG/commit/9af9a21))
* ci: fix env to works with dollar sign ([25266d0](https://github.com/repiazza/CCG/commit/25266d0))
* ci: fix refs to artifact ([c1e5642](https://github.com/repiazza/CCG/commit/c1e5642))
* ci: include package missing release-notes-generator needed by semver ([a50f035](https://github.com/repiazza/CCG/commit/a50f035))
* ci: include package.json needed for version.yml ([5ad6b21](https://github.com/repiazza/CCG/commit/5ad6b21))
* ci: include xml package in install step ([6ca4b7d](https://github.com/repiazza/CCG/commit/6ca4b7d))
* ci: npx needed included ([3d374bc](https://github.com/repiazza/CCG/commit/3d374bc))
* ci: re-enable version.yml pipeline with Windows builds ([4a22b16](https://github.com/repiazza/CCG/commit/4a22b16))
* ci: remove dev-build.yml ([ef4ee62](https://github.com/repiazza/CCG/commit/ef4ee62))
* ci: require all tests to pass before semantic-release ([0ba16fb](https://github.com/repiazza/CCG/commit/0ba16fb))
* ci: run all pipeline jobs on PRs to main ([e885a77](https://github.com/repiazza/CCG/commit/e885a77))
* ci: run full pipeline only for PRs to main ([694e681](https://github.com/repiazza/CCG/commit/694e681))
* ci: skip dev-build pipeline for Docker-only changes ([1568325](https://github.com/repiazza/CCG/commit/1568325))
* ci: trigger full pipeline on push to main ([f81f524](https://github.com/repiazza/CCG/commit/f81f524))
* feat - add trigger ([39f8d8b](https://github.com/repiazza/CCG/commit/39f8d8b))
* feat - CI/CD Docker image ([23d081e](https://github.com/repiazza/CCG/commit/23d081e))
* feat - CI/CD Docker image ([6b2d990](https://github.com/repiazza/CCG/commit/6b2d990))
* feat - CI/CD Docker image ([78b881a](https://github.com/repiazza/CCG/commit/78b881a))
* Feat - Valgrind Supress sdl etc... ([9cdda88](https://github.com/repiazza/CCG/commit/9cdda88))
* feat!: Criacao de modulo cmdline e load dos parametros globais por XML ([27b8d7a](https://github.com/repiazza/CCG/commit/27b8d7a))
* Images ([69ef54c](https://github.com/repiazza/CCG/commit/69ef54c))
* Merge branch 'main' of https://github.com/LampiaoTechTeam/CCG ([ef73b78](https://github.com/repiazza/CCG/commit/ef73b78))
* Merge branch 'main' of https://github.com/repiazza/CCG ([5b1c3f6](https://github.com/repiazza/CCG/commit/5b1c3f6))
* Merge pull request #10 from LampiaoTechTeam/test/pipeline-trigger ([8397834](https://github.com/repiazza/CCG/commit/8397834)), closes [#10](https://github.com/repiazza/CCG/issues/10)
* Merge pull request #11 from LampiaoTechTeam/fix/add-file-utility-to-docker ([e326396](https://github.com/repiazza/CCG/commit/e326396)), closes [#11](https://github.com/repiazza/CCG/issues/11)
* Merge pull request #12 from LampiaoTechTeam/test/trigger-full-pipeline ([8a5ba1c](https://github.com/repiazza/CCG/commit/8a5ba1c)), closes [#12](https://github.com/repiazza/CCG/issues/12)
* chore: added context save function on game.c ([b75a40a](https://github.com/repiazza/CCG/commit/b75a40a))
* chore: remove ftccg ([2eaf2c6](https://github.com/repiazza/CCG/commit/2eaf2c6))
* chore: removed wrong .supp ([020a8af](https://github.com/repiazza/CCG/commit/020a8af))
* chore: removed wrong png ([e6a4d3d](https://github.com/repiazza/CCG/commit/e6a4d3d))
* fix: add file utility to dev Docker image ([2193a0f](https://github.com/repiazza/CCG/commit/2193a0f))
* fix: added .dat game binary context to git ignore ([e1ff772](https://github.com/repiazza/CCG/commit/e1ff772))
* fix: added build dir to git ignore ([39c6722](https://github.com/repiazza/CCG/commit/39c6722))
* fix: added debug level to traces ([260fd73](https://github.com/repiazza/CCG/commit/260fd73))
* fix: added max path definition to path declaration ([972ccb0](https://github.com/repiazza/CCG/commit/972ccb0))
* fix: adicionado novas regras no gitignore ([56d0740](https://github.com/repiazza/CCG/commit/56d0740))
* fix: adjustment on dir creation and validation ([acb6451](https://github.com/repiazza/CCG/commit/acb6451))
* fix: Compilacao para slackware 14.1 ([3bbc3e4](https://github.com/repiazza/CCG/commit/3bbc3e4))
* fix: compilation for slackware ([bd69809](https://github.com/repiazza/CCG/commit/bd69809))
* fix: Correcao no doc: do Make file e alteracoes para melhorar tempo de compilacao ([fdde1f6](https://github.com/repiazza/CCG/commit/fdde1f6))
* fix: Correcao no game load no menu de abertura e correcoes em tamanhos de estruturas ([71ab130](https://github.com/repiazza/CCG/commit/71ab130))
* fix: Corrigido erro de compilacao ([e63bfda](https://github.com/repiazza/CCG/commit/e63bfda))
* fix: declarations on sdl ([fd063c4](https://github.com/repiazza/CCG/commit/fd063c4))
* fix: few adjusts on log functions and SDL x86_64 vs x86 compatibility ([63a1b04](https://github.com/repiazza/CCG/commit/63a1b04))
* fix: handle fread return value in iGameLoad ([c868a37](https://github.com/repiazza/CCG/commit/c868a37))
* fix: log dir creation errors ([f17440c](https://github.com/repiazza/CCG/commit/f17440c))
* fix: Makefile and trace ([29dc674](https://github.com/repiazza/CCG/commit/29dc674))
* fix: non SDL compilation error ([e7c1e6b](https://github.com/repiazza/CCG/commit/e7c1e6b))
* fix: owner Dockerfile ([166a96b](https://github.com/repiazza/CCG/commit/166a96b))
* fix: reallowed semantic-release on push ([59c0645](https://github.com/repiazza/CCG/commit/59c0645))
* fix: Redraw da tela ao sair do menu de pause ([af79bdd](https://github.com/repiazza/CCG/commit/af79bdd))
* fix: removed analyzer ([97ae23e](https://github.com/repiazza/CCG/commit/97ae23e))
* fix: removendo -I/mingw64 da compilacao de linux ([349b167](https://github.com/repiazza/CCG/commit/349b167))
* fix: removendo warning do screen.c em compilacoes para debug ([a50032e](https://github.com/repiazza/CCG/commit/a50032e))
* fix: Removido chamada de codigo reduntante ([81627ff](https://github.com/repiazza/CCG/commit/81627ff))
* fix: Some issues on console mode, xmls and trace ([1edd14c](https://github.com/repiazza/CCG/commit/1edd14c))
* fix: some issues with windows xml reading ([5e3eed4](https://github.com/repiazza/CCG/commit/5e3eed4))
* fix: tab col size on Makefile ([04e440c](https://github.com/repiazza/CCG/commit/04e440c))
* fix: Update README with project and temp directory structure (#13) ([a06b63f](https://github.com/repiazza/CCG/commit/a06b63f)), closes [#13](https://github.com/repiazza/CCG/issues/13)
* fix: URL fixed ([579b30c](https://github.com/repiazza/CCG/commit/579b30c))
* fix: usando _MAX_PATH no szTrace da struct global ([ac85e91](https://github.com/repiazza/CCG/commit/ac85e91))
* fix: use lowercase image name for Docker registry ([d7748af](https://github.com/repiazza/CCG/commit/d7748af))
* fix: Volta de parametro para log no termianl e correcao em bugs nas imagens no linux ([494949a](https://github.com/repiazza/CCG/commit/494949a))
* feat: add dev-build pipeline and disable version pipeline ([c351ddb](https://github.com/repiazza/CCG/commit/c351ddb))
* feat: adjusted some animation errors ([fbd3c7e](https://github.com/repiazza/CCG/commit/fbd3c7e))
* feat: Compilacao para apple ([55af4a8](https://github.com/repiazza/CCG/commit/55af4a8))
* feat: Criacao de screen.xml para load dinamico dos layouts ([3e6adec](https://github.com/repiazza/CCG/commit/3e6adec))
* feat: Criado modulo msg e documentacoes no codigo ([2ed700d](https://github.com/repiazza/CCG/commit/2ed700d))
* feat: Criado modulo msg e documentacoes no codigo ([019235c](https://github.com/repiazza/CCG/commit/019235c))
* feat: Criado parametro para habilitar log no terminal ([e58b2c0](https://github.com/repiazza/CCG/commit/e58b2c0))
* feat: Criado parse para XML; melhorias na compilacao; criacao de maquina de estados; modularizacao d ([bf14152](https://github.com/repiazza/CCG/commit/bf14152))
* feat: Criado tag action para os elementos e os itens dos elementos ([24492e0](https://github.com/repiazza/CCG/commit/24492e0))
* feat: images adjusted ([442c66e](https://github.com/repiazza/CCG/commit/442c66e))
* feat: implemented root dir standarization ([4ec9961](https://github.com/repiazza/CCG/commit/4ec9961))
* feat: load and draw player cards with type ([230324f](https://github.com/repiazza/CCG/commit/230324f))
* doc: remove wrong PNG document ([48f6192](https://github.com/repiazza/CCG/commit/48f6192))
* test: trigger full pipeline lifecycle ([b95d0b0](https://github.com/repiazza/CCG/commit/b95d0b0))
* test: trigger pipeline ([1f1ca78](https://github.com/repiazza/CCG/commit/1f1ca78))
* test: trigger pipeline test ([8a16458](https://github.com/repiazza/CCG/commit/8a16458))
* refactor: move config files to .github/confs/ ([0ccd7c0](https://github.com/repiazza/CCG/commit/0ccd7c0))

## [9.2.6](https://github.com/LampiaoTechTeam/CCG/compare/v9.2.5...v9.2.6) (2025-12-14)

### Bug Fixes

* memory leaks in dialog and SDL cleanup ([3ec6d50](https://github.com/LampiaoTechTeam/CCG/commit/3ec6d508358febe3e269e1e18d3e3ff5def2f1b9))

## [9.2.5](https://github.com/LampiaoTechTeam/CCG/compare/v9.2.4...v9.2.5) (2025-12-07)

### Bug Fixes

* groselha ;) ([cef9a6b](https://github.com/LampiaoTechTeam/CCG/commit/cef9a6ba4d93c7e5e930d5caebd12dcd2fc72587))

## [9.2.4](https://github.com/LampiaoTechTeam/CCG/compare/v9.2.3...v9.2.4) (2025-12-07)

### Bug Fixes

* error on render ([e44c2c9](https://github.com/LampiaoTechTeam/CCG/commit/e44c2c90288f839d19c734f1cd80e3b810cbc4cf))

## [9.2.3](https://github.com/LampiaoTechTeam/CCG/compare/v9.2.2...v9.2.3) (2025-12-07)

### Bug Fixes

* changes to memory otimization ([b63624c](https://github.com/LampiaoTechTeam/CCG/commit/b63624c480e1efd9356a82e8f2d5f04678538070))

## [9.2.2](https://github.com/LampiaoTechTeam/CCG/compare/v9.2.1...v9.2.2) (2025-11-29)

### Bug Fixes

* ignored doxygen doc folder recursively ([35b23d5](https://github.com/LampiaoTechTeam/CCG/commit/35b23d50055447e8fef495485a0ce4924bf64a9e))

## [9.2.1](https://github.com/LampiaoTechTeam/CCG/compare/v9.2.0...v9.2.1) (2025-11-29)

### Bug Fixes

* Fix error in make doc ([bb117df](https://github.com/LampiaoTechTeam/CCG/commit/bb117dfbc2d28fcc5d62cc678cac309ca904df72))

## [9.2.0](https://github.com/LampiaoTechTeam/CCG/compare/v9.1.1...v9.2.0) (2025-11-29)

### Features

* Created GAME.dat and ccg.conf file and parser ([3ee6996](https://github.com/LampiaoTechTeam/CCG/commit/3ee6996dd122310dedef54d234cbc42f4c67d4eb))

### Bug Fixes

* Changed incorrect variable name in ccg.conf ([bb0d46d](https://github.com/LampiaoTechTeam/CCG/commit/bb0d46daaf6e9cdfcb30646aaecbb03bec638273))

## [9.1.1](https://github.com/LampiaoTechTeam/CCG/compare/v9.1.0...v9.1.1) (2025-11-29)

### Bug Fixes

* adjusted sdl redraw actions on table ([0d06c4f](https://github.com/LampiaoTechTeam/CCG/commit/0d06c4f9246096f3e866ce5d13430b226cff7fe1))

## [9.1.0](https://github.com/LampiaoTechTeam/CCG/compare/v9.0.2...v9.1.0) (2025-11-29)

### Features

* Added Doxyfile ([d25ddf0](https://github.com/LampiaoTechTeam/CCG/commit/d25ddf092607b0ad61124ed2e789cf84475cc2bb))
* Added mkdoc.sh ([88e4910](https://github.com/LampiaoTechTeam/CCG/commit/88e491023f52b2c282cb74189ebfd642b60fd7f6))
* Created basic state machine ([fdb0fc3](https://github.com/LampiaoTechTeam/CCG/commit/fdb0fc3fb9a77769f74b3116a7321f110639e748))
* fix pause status ([5af9111](https://github.com/LampiaoTechTeam/CCG/commit/5af9111be63ad8ccc30e65529b74355199377564))

### Bug Fixes

* Merge branch 'gsb' ([b959b87](https://github.com/LampiaoTechTeam/CCG/commit/b959b87cc79811c33eda3f1c9b6414796649d463))

## [9.0.2](https://github.com/LampiaoTechTeam/CCG/compare/v9.0.1...v9.0.2) (2025-11-29)

### Bug Fixes

* Fix strike card ([a940855](https://github.com/LampiaoTechTeam/CCG/commit/a940855567214f02bad227a171c25f1cfd3c478a))

## [9.0.1](https://github.com/LampiaoTechTeam/CCG/compare/v9.0.0...v9.0.1) (2025-11-29)

### Bug Fixes

* few adjustments ([181d5f6](https://github.com/LampiaoTechTeam/CCG/commit/181d5f6ddc0250049633972f607eb7ad9e42a363))

## [9.0.0](https://github.com/LampiaoTechTeam/CCG/compare/v8.2.2...v9.0.0) (2025-11-29)

### ⚠ BREAKING CHANGES

* Huge Modularization

### Features

* Huge Modularization ([f807869](https://github.com/LampiaoTechTeam/CCG/commit/f807869844c1f33296ea27a0f558aaab2af3aff7))

## [8.2.2](https://github.com/LampiaoTechTeam/CCG/compare/v8.2.1...v8.2.2) (2025-11-17)

### Bug Fixes

* fixed references to windows artifacts in .releaserc.json ([2a03271](https://github.com/LampiaoTechTeam/CCG/commit/2a03271becf5423730ba7a32801a17bf393d434c))

## [8.2.1](https://github.com/LampiaoTechTeam/CCG/compare/v8.2.0...v8.2.1) (2025-11-11)

### Bug Fixes

* image.h deps and sdl compat ([ec6be83](https://github.com/LampiaoTechTeam/CCG/commit/ec6be833b0ce40f731b02a864862c910874c95d3))

## [8.2.0](https://github.com/LampiaoTechTeam/CCG/compare/v8.1.0...v8.2.0) (2025-11-11)

### Features

* added shop, levels and more ([6c5410f](https://github.com/LampiaoTechTeam/CCG/commit/6c5410f0851d56565c89368fd15934e99ca6722a))

## [8.1.0](https://github.com/LampiaoTechTeam/CCG/compare/v8.0.1...v8.1.0) (2025-11-08)

### Features

* dialog scroll  and coloured player hud ([7295a97](https://github.com/LampiaoTechTeam/CCG/commit/7295a971776ee81acc13b46c6be3c6f7187888e9))

### Bug Fixes

* console version ([3ddcc25](https://github.com/LampiaoTechTeam/CCG/commit/3ddcc254e88812f77dfa34b08a64be93bfe15f43))
* corrigido erro na compilacao de SDL2 para linux ([607f3c6](https://github.com/LampiaoTechTeam/CCG/commit/607f3c6e91c18ea79d34c059b52f8b00346835b8))
* makefile CCOPT for linux ([5677d50](https://github.com/LampiaoTechTeam/CCG/commit/5677d50fe8dfeebb6ac3b334fcf4d0a95139985e))

## [8.0.1](https://github.com/repiazza/CCG/compare/v8.0.0...v8.0.1) (2025-10-12)

### Bug Fixes

* created linux SDL scripts ([ef1f748](https://github.com/repiazza/CCG/commit/ef1f74897b723faff719710d0b50ce5749d89ee2))

## [8.0.0](https://github.com/repiazza/CCG/compare/v7.2.0...v8.0.0) (2025-10-11)

### ⚠ BREAKING CHANGES

* card flip animation

### Features

* card flip animation ([8ff8bab](https://github.com/repiazza/CCG/commit/8ff8bab82e89cfa6ee980151feb35079305d5dff))

## [7.2.0](https://github.com/repiazza/CCG/compare/v7.1.0...v7.2.0) (2025-10-11)

### Features

* New battle system SDL ([7a895f4](https://github.com/repiazza/CCG/commit/7a895f417b86bbbbd7599381b9d025dc79d82e2a))

## [7.1.0](https://github.com/repiazza/CCG/compare/v7.0.0...v7.1.0) (2025-10-03)

### Features

* SDL dialog ([0c91268](https://github.com/repiazza/CCG/commit/0c912685ae570e1568fa7535f5aa8db6a4c8d0bd))

## [7.0.0](https://github.com/repiazza/CCG/compare/v6.0.0...v7.0.0) (2025-10-03)

### ⚠ BREAKING CHANGES

* changes on main sdl interface

### Features

* changes on main sdl interface ([7a88cec](https://github.com/repiazza/CCG/commit/7a88cec00e6015e94f13ea53a5020898beaedbcd))

## [6.0.0](https://github.com/repiazza/CCG/compare/v5.1.1...v6.0.0) (2025-10-03)

### ⚠ BREAKING CHANGES

* included initial table drawing

### Features

* included initial table drawing ([8c1b935](https://github.com/repiazza/CCG/commit/8c1b9353b6447581469f5445b1d41d0482f9a037))

## [5.1.1](https://github.com/repiazza/CCG/compare/v5.1.0...v5.1.1) (2025-10-01)

### Bug Fixes

* included ctype header on input.c ([67977a4](https://github.com/repiazza/CCG/commit/67977a4bacfdbfcd5a4834aa514f3d097f264b9c))

## [5.1.0](https://github.com/repiazza/CCG/compare/v5.0.0...v5.1.0) (2025-09-27)

### Features

* some adjustments on main ([5babb61](https://github.com/repiazza/CCG/commit/5babb61b6c1f7859811000a7f585723c499809db))

## [5.0.0](https://github.com/repiazza/CCG/compare/v4.1.2...v5.0.0) (2025-09-27)

### ⚠ BREAKING CHANGES

* major SDL2 + console api

### Features

* major SDL2 + console api ([3fb8365](https://github.com/repiazza/CCG/commit/3fb83659bd1a37b7b8f977ebd1217d0189c34b97))

### Bug Fixes

* include header ([9e92e5b](https://github.com/repiazza/CCG/commit/9e92e5bfe4c77bde02b19233fb529ae13ad642ef))

## [4.1.2](https://github.com/repiazza/CCG/compare/v4.1.1...v4.1.2) (2025-09-20)

### Bug Fixes

* battle and monsters ([e9fe56c](https://github.com/repiazza/CCG/commit/e9fe56cd2c4b46b31eb15c2059e440cbc718fb23))

## [4.1.1](https://github.com/repiazza/CCG/compare/v4.1.0...v4.1.1) (2025-09-20)

### Bug Fixes

* Adjustments on alignment interface ([fe2f9e1](https://github.com/repiazza/CCG/commit/fe2f9e1c3ef0ebff152b7444a0173b29e974ac76))

## [4.1.0](https://github.com/repiazza/CCG/compare/v4.0.0...v4.1.0) (2025-09-20)

### Features

* paralize accumulated ([60660d0](https://github.com/repiazza/CCG/commit/60660d0feda079500aeca6a3eda9b5d2ed323330))

## [4.0.0](https://github.com/repiazza/CCG/compare/v3.3.1...v4.0.0) (2025-09-20)

### ⚠ BREAKING CHANGES

* merged branches and corrections

### Features

* merged branches and corrections ([a168c53](https://github.com/repiazza/CCG/commit/a168c53cc8f01656f1be233a944defd2431bcb14))

## [3.3.1](https://github.com/repiazza/CCG/compare/v3.3.0...v3.3.1) (2025-09-20)

### Bug Fixes

* waning on trace ([50f424e](https://github.com/repiazza/CCG/commit/50f424e97cad7bfe529d46ec73eb59467dc8c030))

## [3.3.0](https://github.com/repiazza/CCG/compare/v3.2.0...v3.3.0) (2025-09-20)

### Features

* adjusted monster selection ([735c528](https://github.com/repiazza/CCG/commit/735c528790711d5ef440ad4bfb405621b73c99dc))

### Bug Fixes

* merge branch 'main' into fix-commits ([4cc8b94](https://github.com/repiazza/CCG/commit/4cc8b94c9a524b0a6839de100437bf0d91be7928))

## [3.0.0](https://github.com/repiazza/CCG/compare/v2.0.0...v3.0.0) (2025-09-19)

### ⚠ BREAKING CHANGES

* Several changes on battle

### Features

* Several changes on battle ([870ef6e](https://github.com/repiazza/CCG/commit/870ef6ef58b6924ca58ca5991f94f9c38d128fa6))

## [2.0.0](https://github.com/repiazza/CCG/compare/v1.0.1...v2.0.0) (2025-09-14)

### ⚠ BREAKING CHANGES

* several changes

### Features

* several changes ([a44beeb](https://github.com/repiazza/CCG/commit/a44beeb9ad4c973fea63f554d407939c7d05c240))

## [1.0.1](https://github.com/repiazza/CCG/compare/v1.0.0...v1.0.1) (2025-09-13)

### Bug Fixes

* **actions:** changed step position and CHANGELOG is up-to-date ([5ea138d](https://github.com/repiazza/CCG/commit/5ea138dab57f67fb2c877e1a64821e648f8afdbc))

## [1.0.0](https://github.com/repiazza/CCG/compare/v0.0.1...v1.0.0) (2025-09-13)

### ⚠ BREAKING CHANGES

- new dialog

### Features

- new dialog ([7bfea13](https://github.com/repiazza/CCG/commit/7bfea13b0b1726c12f41df30020ee2fef20b6c9c))

## [0.0.1](https://github.com/repiazza/CCG/compare/v0.0.0...v0.0.1) (2025-09-13)

### Bug Fixes

- **semver:** removed create-release step and fixed minor errors ([08d9867](https://github.com/repiazza/CCG/commit/08d9867a4958b1dc5af303b56276b1438b86c42e))
