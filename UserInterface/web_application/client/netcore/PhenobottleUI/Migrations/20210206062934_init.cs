using Microsoft.EntityFrameworkCore.Migrations;

namespace PhenobottleUI.Migrations
{
    public partial class init : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Phenobottles",
                columns: table => new
                {
                    ID = table.Column<string>(type: "text", nullable: false),
                    Treatment = table.Column<string>(type: "text", nullable: true),
                    ReplicateNumber = table.Column<string>(type: "text", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Phenobottles", x => x.ID);
                });
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Phenobottles");
        }
    }
}
