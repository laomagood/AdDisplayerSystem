#ifndef GLOBAL_H
#define GLOBAL_H

/**
 * @brief Global
 * 全局变量
 */

const bool g_isReleaseProgram = true;    // 节目是否在发布状态（server制作：false client发布：true）

// 用户管理列枚举变量
enum UserManagementColumn { CheckBoxColumn, SerialNumberColumn, LoginNameColumn,
                            GenerateTimeColumn, LoginTimeColumn, LastLoginTimeColumn,
                            RoleColumn, EditColumn };

// 角色管理列枚举变量
namespace RoleManagement
{
enum RoleManagementColumn { CheckBoxColumn, SerialNumberColumn, RoleNameColumn,
                            GenerateTimeColumn, UpdateTimeColumn, DescriptionColumn,
                            EditColumn };
}

// 文件类型列枚举变量
enum FileType { AllType, ImageType, VideoType, AudioType,
                DocumentType, OtherType, VoidType, ProgramType };


#endif // GLOBAL_H

