#include <QTest>
#include <QDebug>
#include <sstream>
#include <Base/Builder3D.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoSeparator.h>

class testInventorBuilder : public QObject
{
    Q_OBJECT

public:
    testInventorBuilder()
        : builder(output)
    {
    }
    ~testInventorBuilder()
    {
    }

    SoNode* loadBuffer(const std::string& buffer)
    {
        SoInput in;
        in.setBuffer((void *)buffer.c_str(), buffer.size());
        return SoDB::readAll(&in);
    }

private Q_SLOTS:
    void initTestCase()
    {
        SoDB::init();
    }
    void initTestCase_data()
    {

    }
    void cleanupTestCase()
    {
        SoDB::finish();
    }

    void init()
    {
    }

    void cleanup()
    {
        // clear the buffer
        output.str(std::string());
    }

    void test_Invalid()
    {
        SoNode* node = loadBuffer("Hello, World");
        QCOMPARE(node, nullptr);
    }

    void test_Output()
    {
        std::stringstream str;
        Base::InventorBuilder builder(str);
        QCOMPARE(str.str().c_str(), "#Inventor V2.1 ascii \n\n");
    }

    void test_MaterialBinding_data()
    {
        QTest::addColumn<QString>("result");
        QTest::newRow("MaterialBinding") << "MaterialBinding { value OVERALL } \n";
    }

    void test_MaterialBinding()
    {
        QFETCH(QString, result);

        Base::MaterialBindingItem item;
        builder.addNode(item);
        QString string = QString::fromStdString(output.str());

        QCOMPARE(string, result);
    }

    void test_Label_data()
    {
        auto result =
R"(Label {
  label "FreeCAD"
}
)";
        QTest::addColumn<QString>("result");
        QTest::newRow("Label") << result;
    }

    void test_Label()
    {
        QFETCH(QString, result);

        Base::LabelItem item{"FreeCAD"};
        builder.addNode(item);
        QString string = QString::fromStdString(output.str());

        QCOMPARE(string, result);
    }

    void test_Info_data()
    {
        auto result =
R"(Info {
  string "FreeCAD"
}
)";
        QTest::addColumn<QString>("result");
        QTest::newRow("Label") << result;
    }

    void test_Info()
    {
        QFETCH(QString, result);

        Base::InfoItem item{"FreeCAD"};
        builder.addNode(item);
        QString string = QString::fromStdString(output.str());

        QCOMPARE(string, result);
    }

    void test_BaseColor_data()
    {
        auto result =
R"(BaseColor {
  rgb 0.21 0.3 0.4
}
)";
        QTest::addColumn<QString>("result");
        QTest::newRow("BaseColor") << result;
    }

    void test_BaseColor()
    {
        QFETCH(QString, result);

        Base::BaseColorItem item{Base::ColorRGB{0.21F, 0.3F, 0.4F}};
        builder.addNode(item);
        QString string = QString::fromStdString(output.str());

        QCOMPARE(string, result);
    }

    void test_Material_data()
    {
        auto result =
R"(Material {
  diffuseColor 1 0 0
}
)";
        QTest::addColumn<QString>("result");
        QTest::newRow("Material") << result;
    }

    void test_Material()
    {
        QFETCH(QString, result);

        Base::MaterialItem item;
        item.setDiffuseColor({Base::ColorRGB{1,0,0}});
        builder.addNode(item);
        QString string = QString::fromStdString(output.str());

        QCOMPARE(string, result);
    }

    void test_Materials_data()
    {
        auto result =
R"(Material {
  diffuseColor [
    1 0 0
    0 1 0
    0 0 1
  ]
}
)";
        QTest::addColumn<QString>("result");
        QTest::newRow("Material") << result;
    }

    void test_Materials()
    {
        QFETCH(QString, result);

        Base::MaterialItem item;
        item.setDiffuseColor({Base::ColorRGB{1,0,0},
                              Base::ColorRGB{0,1,0},
                              Base::ColorRGB{0,0,1}});
        builder.addNode(item);
        QString string = QString::fromStdString(output.str());

        QCOMPARE(string, result);

        SoNode* node = loadBuffer(output.str());
        QVERIFY(node != nullptr);
        QVERIFY(node->getRefCount() == 0);
    }

    void test_DrawStyle_data()
    {
        auto result =
R"(DrawStyle {
  style FILLED
  pointSize 2
  lineWidth 2
  linePattern 0xffff
}
)";
        QTest::addColumn<QString>("result");
        QTest::newRow("DrawStyle") << result;
    }

    void test_DrawStyle()
    {
        QFETCH(QString, result);

        Base::DrawStyleItem item{Base::DrawStyle{}};
        builder.addNode(item);
        QString string = QString::fromStdString(output.str());

        QCOMPARE(string, result);
    }

    void test_ShapeHints_data()
    {
        auto result =
R"(ShapeHints {
  creaseAngle 0.5
}
)";
        QTest::addColumn<QString>("result");
        QTest::newRow("ShapeHints") << result;
    }

    void test_ShapeHints()
    {
        QFETCH(QString, result);

        Base::ShapeHintsItem item{0.5F};
        builder.addNode(item);
        QString string = QString::fromStdString(output.str());

        QCOMPARE(string, result);
    }

    void test_PolygonOffset_data()
    {
        auto result =
R"(PolygonOffset {
  factor 1
  units 1
  styles FILLED
  on TRUE
}
)";
        QTest::addColumn<QString>("result");
        QTest::newRow("PolygonOffset") << result;
    }

    void test_PolygonOffset()
    {
        QFETCH(QString, result);

        Base::PolygonOffsetItem item{Base::PolygonOffset{}};
        builder.addNode(item);
        QString string = QString::fromStdString(output.str());

        QCOMPARE(string, result);
    }

    void test_PointSet_data()
    {
        auto result = "PointSet { }\n";
        QTest::addColumn<QString>("result");
        QTest::newRow("PointSet") << result;
    }

    void test_PointSet()
    {
        QFETCH(QString, result);

        Base::PointSetItem item;
        builder.addNode(item);
        QString string = QString::fromStdString(output.str());

        QCOMPARE(string, result);
    }

    void test_Normal_data()
    {
        auto result =
R"(Normal {
  vector 1 0 0.5
}
)";
        QTest::addColumn<QString>("result");
        QTest::newRow("Normal") << result;
    }

    void test_Normal()
    {
        QFETCH(QString, result);

        Base::NormalItem item;
        item.setVector({Base::Vector3f{1,0,0.5}});
        builder.addNode(item);
        QString string = QString::fromStdString(output.str());

        QCOMPARE(string, result);
    }

    void test_LineItem()
    {
        Base::Line3f line;
        Base::DrawStyle style;
        Base::LineItem item{line, style};
        builder.addNode(item);

        SoNode* node = loadBuffer(output.str());
        QVERIFY(node != nullptr);
    }

    void test_ArrowItem()
    {
        Base::Line3f line;
        line.p2.z = 10;
        Base::DrawStyle style;
        Base::ArrowItem item{line, style};
        builder.addNode(item);

        qDebug() << QString::fromStdString(output.str());
        SoNode* node = loadBuffer(output.str());
        QVERIFY(node != nullptr);
    }

    void test_PointItem()
    {
        Base::Vector3f pnt;
        Base::DrawStyle style;
        Base::PointItem item{pnt, style};
        builder.addNode(item);

        SoNode* node = loadBuffer(output.str());
        QVERIFY(node != nullptr);
    }

    void test_NormalBinding_data()
    {
        auto result = "NormalBinding { value PER_PART }\n";
        QTest::addColumn<QString>("result");
        QTest::newRow("NormalBinding") << result;
    }

    void test_NormalBinding()
    {
        QFETCH(QString, result);

        Base::NormalBindingItem item;
        item.setValue(Base::BindingElement::Binding::PerPart);
        builder.addNode(item);
        QString string = QString::fromStdString(output.str());

        QCOMPARE(string, result);
    }

    void test_Cylinder_data()
    {
        auto result =
R"(Cylinder {
  radius 3
  height 7
  parts (SIDES | TOP | BOTTOM)
}
)";
        QTest::addColumn<QString>("result");
        QTest::newRow("Cylinder") << result;
    }

    void test_Cylinder()
    {
        QFETCH(QString, result);

        Base::CylinderItem item;
        item.setRadius(3);
        item.setHeight(7);
        builder.addNode(item);
        QString string = QString::fromStdString(output.str());

        QCOMPARE(string, result);
    }

    void test_Cone_data()
    {
        auto result = "Cone { bottomRadius 2 height 10 }\n";
        QTest::addColumn<QString>("result");
        QTest::newRow("Cone") << result;
    }

    void test_Cone()
    {
        QFETCH(QString, result);

        Base::ConeItem item;
        item.setBottomRadius(2);
        item.setHeight(10);
        builder.addNode(item);
        QString string = QString::fromStdString(output.str());

        QCOMPARE(string, result);
    }

    void test_Sphere_data()
    {
        auto result = "Sphere { radius 4 }\n";
        QTest::addColumn<QString>("result");
        QTest::newRow("Sphere") << result;
    }

    void test_Sphere()
    {
        QFETCH(QString, result);

        Base::SphereItem item;
        item.setRadius(4);
        builder.addNode(item);
        QString string = QString::fromStdString(output.str());

        QCOMPARE(string, result);
    }

private:
    std::stringstream output;
    Base::InventorBuilder builder;
};

QTEST_GUILESS_MAIN(testInventorBuilder)

#include "InventorBuilder.moc"
