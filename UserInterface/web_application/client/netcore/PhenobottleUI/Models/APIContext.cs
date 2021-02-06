
using Microsoft.EntityFrameworkCore;
using System;

namespace PhenobottleUI.Models
{
    public class APIContext : DbContext
    { 
		public APIContext(DbContextOptions<APIContext> options) : base(options){ 
		
		}
		public DbSet<Phenobottle> Phenobottles { get; set; }
    }
}
