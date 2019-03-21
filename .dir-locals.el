;;; Directory Local Variables
;;; For more information see (info "(emacs) Directory Variables")

((nil .
      ((cmake-ide-project-dir . "~/Code/C++/ate-chips")
       (cmake-ide-build-dir . "~/Code/C++/ate-chips/build")
       (cmake-ide-cmake-opts . "-DCMAKE_BUILD_TYPE=Debug")
       (helm-make-build-dir . "build")
       (helm-make-arguments . "-j9")
       (helm-ctest-dir . "~/Code/C++/ate-chips/build/")
       )))

((nil . ((eval . (setq
                  projectile-project-test-cmd #'helm-ctest
                  projectile-project-compilation-cmd #'helm-make-projectile
                  projectile-project-compilation-dir "build"
                  helm-make-build-dir (projectile-compilation-dir)
                  helm-ctest-dir (projectile-compilation-dir)
                  ))
         (projectile-project-name . "ate-chips")
         (projectile-project-run-cmd . "./pf-test-project")
         (projectile-project-configure-cmd . "cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..")
         (helm-make-arguments . "-j9"))))
