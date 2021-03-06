/*
*  Author: gatieme @ HIT CS HDMC team.
*  Create: 2016-3-12 8:50
*  Last modified: 2010-6-13 14:06:20
*  Description:
*
*   Our project was completed in 2010 @ v1.0.0
*   This module is create by gatieme @2015-11-29
*
*  	Memory fault injection engine running as a kernel module.
*	This module will create "/proc/memoryEngine/" directory and 9 proc nodes.
*   Write paramenters and request to these proc nodes and read the output from related proc node.
*/

#ifndef _MEMORY_ENGINE_H
#define _MEMORY_ENGINE_H

/*
*  common include header files
*/


#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/kprobes.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/mount.h>
#include <linux/dcache.h>
#include <linux/slab.h>
#include <linux/highmem.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/kallsyms.h>
#include <linux/kprobes.h>
#include <asm/pgtable.h>
#include <asm/page.h>

#include "common.h"


#define MAX_LINE		        256						/// max length of a line
#define DELIMITER		        '\n'					/// the char is used to split the taskinfo
#define PERMISSION	            0644					/// proc node permission
/*
*	return values
*/
#define OK					     0
#define FAIL					-1
#define PGD_NONE 			    -2
#define PUD_NONE			    -3
#define PMD_NONE 			    -4
#define PTE_NONE 			    -5
#define PTE_NOT_PRESENT         -6



/*
*	request command
*/
#define REQUEST_TASK_INFO   	1		/// get a task's memory map information
#define REQUEST_V2P				2		/// convert a process's linear address to physical address
#define REQUEST_KV2P		    3		/// convert kernel virtual address to physical address
#define REQUEST_KFUNC_VA	    4		/// get kernel function's addr(kernel virtual address)
#define REQUEST_READ_KFUNC	    5		/// 请求读取内核函数起始地址内容
#define REQUEST_WRITE_KFUNC	    6		/// 请求改写内核函数起始地址内容

#if defined(RW_pA_BY_KERNEL)
    #define REQUEST_READ_PA		10	    /// 请求获取全部物理内存信息
    #define REQUEST_WRITE_PA	11 	    /// 请求改写指定物理地址内容，改为用户态实现此功能
#endif

#if defined(RW_VA_KERNEL)
    #define REQUEST_READ_VA		12	    /// 请求获取全部物理内存信息
    #define REQUEST_WRITE_VA	13 	    /// 请求改写指定物理地址内容，改为用户态实现此功能
#endif


#define REQUEST_MEM			14	    /// 请求获取全部物理内存信息
#define REQUEST_ADDR_STOP	15	    ///

/*
*	ack signals
*/
#define ACK_TASK_INFO		    REQUEST_TASK_INFO
#define ACK_V2P				   	REQUEST_V2P
#define ACK_KV2P			    REQUEST_KV2P
#define ACK_KFUNC_VA		    REQUEST_KFUNC_VA
#define ACK_READ_KFUNC	        REQUEST_READ_KFUNC
#define ACK_WRITE_KFUNC	        REQUEST_WRITE_KFUNC

#if defined(RW_pA_BY_KERNEL)
    #define ACK_READ_PA         REQUEST_READ_PA
    #define ACK_WRITE_PA        REQUEST_MEM_PA
#endif

#if defined(RW_VA_KERNEL)
    #define ACK_READ_VA         REQUEST_READ_VA
    #define ACK_WRITE_VA        REQUEST_WRITE_VA
#endif

#define ACK_MEM             REQUEST_MEM
#define ACK_ADDR_STOP       REQUEST_ADDR_STOP

/*
*	utility functions
*/
struct task_struct *findTaskByPid(pid_t pid);

long v2p(struct mm_struct *pMM,unsigned long va,int *pStatus);

long kv2p(unsigned long va,int *pStatus);

long kFunc2v(char *funcName);

struct vm_area_struct * getVMA(struct mm_struct *pMM,unsigned long va);

pte_t * getPte(struct mm_struct *pMM,unsigned long va);

int setVMAFlags(struct mm_struct *pMM,unsigned long va,int *pStatus,int flags);

int setPageFlags(struct mm_struct *pMM,unsigned long va,int *pStatus,int flags);

int getTaskInfo(struct task_struct *pTask, char *pData, int length);

/* MODIFY gatieme @ 2016-005-27 19:50
 * FOR read and write physical in kernel space
 */
unsigned long writepa(unsigned long pa);

unsigned long readpa(unsigned long pa);



/*
 * map an virtual memory space `vma` to the kernel space `buffer`
 */
/*
*	proc entry function
*/
int proc_write_pid(struct file *file,const char __user *buffer,unsigned long count,void * data);

int proc_read_va(char * page,char **start, off_t off, int count, int * eof,void * data);
int proc_write_va(struct file *file,const char *buffer,unsigned long count,void * data);

int proc_write_ctl(struct file *file,const char *buffer,unsigned long count,void * data);

int proc_read_signal(char * page,char **start, off_t off, int count, int * eof,void * data);
int proc_write_signal(struct file *file,const char *buffer,unsigned long count,void * data);

/*
 * physical address module
 *
 */
int proc_read_pa(char * page,char **start, off_t off, int count, int * eof,void * data);
int proc_write_pa(struct file *file,const char *buffer,unsigned long count,void * data);

/*
 *  kFuncName module
 *
 */
int proc_write_kFuncName(struct file *file,const char *buffer,unsigned long count,void * data);

/*
 *  taskInfo module
 *
 */
int proc_read_taskInfo(char * page,char **start, off_t off, int count, int * eof,void * data);

/*
 *  memVal module
 *
 */
int proc_read_memVal(char * page, char **start, off_t off, int count, int * eof, void * data);
int proc_write_memVal(struct file *file,const char *buffer,unsigned long count,void * data);



/*
*  process the request
*
*/
void do_request(void);


#endif
