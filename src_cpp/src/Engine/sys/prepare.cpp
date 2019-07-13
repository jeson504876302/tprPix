/*
 * ========================= prepare.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "prepare.h"

//---------------- from cmake ------------------//
#include "SysConfig.h" // MUST BEFORE TPR_OS_XXX macros !!!

//-------------------- C ----------------------//
#if defined TPR_OS_WIN32_
	#include <windows.h>
#elif defined TPR_OS_UNIX_
    #include <limits.h>  //- PATH_MAX
    #include <stdlib.h>  //- realpath
	#include <unistd.h>  //- fchdir
	#include <fcntl.h>    //-- open，openat, AT_FDCWD
#endif

//-------------------- CPP --------------------//
#include <string> 
#include <fstream>   //-- ifstream
#include <sstream>   //-- stringstream

//------------------- Libs --------------------//
#include "tprGeneral.h"


#if defined TPR_OS_WIN32_
    #include "tprFileSys_win.h" 
#elif defined TPR_OS_UNIX_
    #include "wrapUnix.h"
    #include "tprFileModeT.h"
    #include "tprFileSys_unix.h" 
#endif

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "global.h"


#include <iostream>
using std::cout;
using std::endl;



namespace prepare_inn {//------------ namespace: prepare_inn ------------//
    void build_path_cwd( char *exeDirPath_ );
    void data_type_confirm();
    void check_OS();
    //void check_fst_run();
    void check_and_creat_important_dir();
}//------------ namespace: prepare_inn end ------------//



/*===========================================================
 *                      prepare  
 *-----------------------------------------------------------
 */
void prepare( char *exeDirPath_ ){

    //------------------------//
    //      变量类型检测
    //------------------------//
    prepare_inn::data_type_confirm();

    //------------------------//
    //   查看当前所在的 操作系统
    //------------------------//
    prepare_inn::check_OS();

    //------------------------//
    //    初始化 path_cwd
    //------------------------//
    prepare_inn::build_path_cwd( exeDirPath_ );

    //------------------------//
    // 检测 关键 目录 的存在，若没有，创建之
    //------------------------//
    prepare_inn::check_and_creat_important_dir();


    //----------------------------//
    //  检测 是否为 本程序的 首次运行
    //----------------------------//
    //check_fst_run();

    return;
}

namespace prepare_inn {//------------ namespace: prepare_inn ------------//


/* ===========================================================
 *                  build_path_cwd   
 * -----------------------------------------------------------
 */
void build_path_cwd( char *exeDirPath_ ){

    //-- 这部分工作，已经由 C# 完成 ---
    path_cwd = exeDirPath_;
}


/*===========================================================
 *           check_and_creat_important_dir    
 *-----------------------------------------------------------
 */
void check_and_creat_important_dir(){

    std::string err_info = "check_and_creat_important_dir(): ";

    //----------------------------//
    cout << "path_cwd = " << path_cwd << endl;

#if defined TPR_OS_WIN32_

    //---------------------------------//
    //           path_csharpLibs
    //---------------------------------//
    path_csharpLibs = tprWin::mk_dir( path_cwd,
                        "csharpLibs/",
                        err_info );

    //---------------------------------//
    //           path_data
    //---------------------------------//
    path_dataBase = tprWin::mk_dir( path_cwd,
                                "dataBase/",
                                err_info );
    //---------------------------------//
    //           path_shaders
    //---------------------------------//
    path_shaders = tprWin::mk_dir( path_cwd,
                                "shaders/",
                                err_info );
    //---------------------------------//
    //           path_textures
    //---------------------------------//
    path_textures = tprWin::mk_dir( path_cwd,
                                "textures/",
                                err_info );
    //---------------------------------//
    //          path_animFrameSets
    //---------------------------------//
    path_animFrameSets = tprWin::mk_dir( path_textures,
                                    "animFrameSets/",
                                    err_info );
    //---------------------------------//
    //          path_colliEntSet
    //---------------------------------//
    path_colliEntSet = tprWin::mk_dir( path_textures,
                                    "colliEntSet/",
                                    err_info );
    //---------------------------------//
    //          path_fieldBorderSet
    //---------------------------------//
    path_fieldBorderSet = tprWin::mk_dir( path_textures,
                                        "fieldBorderSet/",
                                        err_info );
    //---------------------------------//
    //           path_jsons
    //---------------------------------//
    path_jsons = tprWin::mk_dir( path_cwd,
                                "jsons/",
                                err_info );

#elif defined TPR_OS_UNIX_

    //---------------------------------//
    //           path_csharpLibs
    //---------------------------------//
    path_csharpLibs = tprUnix::mk_dir( path_cwd,
                        "csharpLibs/",
                        RWXR_XR_X,
                        err_info );

    //---------------------------------//
    //           path_data
    //---------------------------------//
    path_dataBase = tprUnix::mk_dir( path_cwd,
                        "dataBase/",
                        RWXR_XR_X,
                        err_info );
    //---------------------------------//
    //           path_shaders
    //---------------------------------//
    path_shaders = tprUnix::mk_dir( path_cwd,
                        "shaders/",
                        RWXR_XR_X,
                        err_info );
    //---------------------------------//
    //           path_textures
    //---------------------------------//
    path_textures = tprUnix::mk_dir( path_cwd,
                        "textures/",
                        RWXR_XR_X,
                        err_info );
    //---------------------------------//
    //          path_animFrameSets
    //---------------------------------//
    path_animFrameSets = tprUnix::mk_dir( path_textures,
                        "animFrameSets/",
                        RWXR_XR_X,
                        err_info );
    //---------------------------------//
    //          path_colliEntSet
    //---------------------------------//
    path_colliEntSet = tprUnix::mk_dir( path_textures,
                        "colliEntSet/",
                        RWXR_XR_X,
                        err_info );
    //---------------------------------//
    //          path_fieldBorderSet
    //---------------------------------//
    path_fieldBorderSet = tprUnix::mk_dir( path_textures,
                        "fieldBorderSet/",
                        RWXR_XR_X,
                        err_info );
    //---------------------------------//
    //           path_jsons
    //---------------------------------//
    path_jsons = tprUnix::mk_dir( path_cwd,
                        "jsons/",
                        RWXR_XR_X,
                        err_info );


#endif
}


/* ===========================================================
 *                  data_type_confirm    
 * -----------------------------------------------------------
 * -- 一种简陋的 类型检测。
 */
void data_type_confirm(){

    int len {}; //-- 临时变量。
    //------- short ------//
    len = sizeof(short);
    if( len != 2 ){
        tprAssert(0);
    }

    //------- unsigned short ------//
    len = sizeof(unsigned short);
    if( len != 2 ){
        tprAssert(0);
    }

    //------- int ------//
    len = sizeof(int);
    if( len != 4 ){
        tprAssert(0);
    }

    //------- unsigned int ------//
    len = sizeof(unsigned int);
    if( len != 4 ){
        tprAssert(0);
    }

    //------- long long ------//
    len = sizeof(long long);
    if( len != 8 ){
        tprAssert(0);
    }

    //------- unsigned long long ------//
    len = sizeof(unsigned long long);
    if( len != 8 ){
        tprAssert(0);
    }

    //------- float ------//
    len = sizeof(float);
    if( len != 4 ){
        tprAssert(0);
    }

    //------- double ------//
    len = sizeof(double);
    if( len != 8 ){
        tprAssert(0);
    }

    //------- off_t ------//
    /*
    len = sizeof(off_t);
    if( len != 8 ){
        //cout << "data_type_confirm: ERROR."
        //    << "off_t len = " << len
        //    << endl;
        assert(0);
    }
    */
         //-- unix 中为 8， win 中为 4
         //   暂不处理...
}


/* ===========================================================
 *                     check_OS
 * -----------------------------------------------------------
 * -- 检测 当前 程序 身处什么 操作系统 
 */
void check_OS(){

    cout << SYSTEM_NAME << endl;

    current_OS = OS_NULL; //-- 先设置为 未找到状态

    #if defined TPR_OS_MACOSX_
        current_OS = OS_APPLE;
        cout << "TPR_OS_MACOSX_" << endl;

    #elif defined TPR_OS_LINUX_
        current_OS = OS_UNIX;
        cout << "TPR_OS_LINUX_" << endl;

    #elif defined TPR_OS_WIN32_
        current_OS = OS_WIN32;
        cout << "TPR_OS_WIN32_" << endl;
    #endif 
    
}


/* ===========================================================
 *                       check_fst_run  
 * -----------------------------------------------------------
 * -- 检查本程序 是否为 编译后 首次运行，如果是，需要执行一些 初始化工作
 */
/*
void check_fst_run(){

    //-- 合成 already_run 文件 绝对路径名 
    std::string path = tpr::path_combine( path_cwd, "/build/already_run.flag" );

    //-- 查看 already_run 文件 是否存在，若存在，open return -1， 若不存在，创建它。
    if( open(path.c_str(),(O_RDONLY | O_CREAT | O_EXCL),RWXR_XR_X) >= 0 ){
        //-- 说明 目标文件 不存在： 本程序第一次运行 --
        //cout << "+++ fst run. +++" << endl;
        is_fst_run = true;

    }else{
        //-- 说明 目标文件 存在： 本程序 不是 第一次运行 --
        //cout << "+++ not fst run. +++" << endl;
        is_fst_run = false;
    }
}
*/


}//------------- namespace: prepare_inn ----------------//
