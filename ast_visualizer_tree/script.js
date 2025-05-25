const canvas = document.getElementById('treeCanvas');
const ctx = canvas.getContext('2d');
const nodeRadius = 20;
const verticalSpacing = 100;
const horizontalSpacing = 40;

document.getElementById('fileInput').addEventListener('change', function(event) {
    const file = event.target.files[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = function(e) {
        try {
            const json = JSON.parse(e.target.result);
            ctx.clearRect(0, 0, canvas.width, canvas.height);

            let treeData = null;
            if (Array.isArray(json.statements) && json.statements.length > 0) {
                treeData = buildTreeData(json.statements[0].ast);
            } else if (json.ast) {
                treeData = buildTreeData(json.ast);
            } else {
                alert('Invalid AST JSON format: missing statements array or ast root.');
                return;
            }

            const treeHeight = getTreeHeight(treeData);
            canvas.height = treeHeight + 100;

            layoutTree(treeData, 50, 50);
            drawTree(treeData);
        } catch (err) {
            alert('Error parsing JSON: ' + err.message);
        }
    };
    reader.readAsText(file);
});

function buildTreeData(node) {
    if (!node) return null;

    let label = node.type || 'UNKNOWN';
    if (node.name) {
        label += `: ${node.name}`;
    } else if (node.value !== undefined && node.value !== null) {
        if (typeof node.value !== 'object') {
            label += `: ${node.value.toString()}`;
        }
    }

    const treeNode = {
        type: node.type || 'UNKNOWN',
        label: label,
        children: []
    };

    for (const key in node) {
        if (key === 'type' || key === 'id' || key === 'next' || key === 'name' || key === 'value') continue;
        const child = node[key];
        if (child === null || child === undefined) continue;

        if (Array.isArray(child)) {
            child.forEach(c => {
                const childNode = buildTreeData(c);
                if (childNode) treeNode.children.push(childNode);
            });
        } else if (typeof child === 'object') {
            const childNode = buildTreeData(child);
            if (childNode) treeNode.children.push(childNode);
        } else {
            treeNode.children.push({
                type: 'Value',
                label: `${key}: ${child.toString()}`,
                children: []
            });
        }
    }

    if (node.next) {
        const nextNode = buildTreeData(node.next);
        if (nextNode) treeNode.children.push(nextNode);
    }

    return treeNode;
}

function layoutTree(node, startX, startY) {
    let x = startX;
    let y = startY;

    if (!node.children || node.children.length === 0) {
        node.x = x;
        node.y = y;
        node.width = nodeRadius * 2;
        node.height = nodeRadius * 2;
        return node.x + node.width + horizontalSpacing;
    }

    let childX = x;
    node.y = y;
    node.children.forEach(child => {
        childX = layoutTree(child, childX, y + verticalSpacing);
    });

    const firstChild = node.children[0];
    const lastChild = node.children[node.children.length - 1];
    node.x = (firstChild.x + lastChild.x) / 2;
    node.width = lastChild.x + nodeRadius * 2 - firstChild.x;
    node.height = verticalSpacing + Math.max(...node.children.map(c => c.height || 0));

    return childX;
}

function drawTree(node) {
    if (!node) return;

    if (node.children) {
        node.children.forEach(child => {
            drawLine(node.x, node.y, child.x, child.y);
            drawTree(child);
        });
    }

    drawNode(node.x, node.y, node.label);
}

function drawLine(x1, y1, x2, y2) {
    ctx.beginPath();
    ctx.moveTo(x1, y1);
    ctx.lineTo(x2, y2);
    ctx.strokeStyle = '#555';
    ctx.lineWidth = 2;
    ctx.stroke();
}

function drawNode(x, y, label) {
    ctx.beginPath();
    ctx.arc(x, y, nodeRadius, 0, 2 * Math.PI);
    ctx.fillStyle = '#4CAF50';
    ctx.fill();
    ctx.strokeStyle = '#333';
    ctx.lineWidth = 2;
    ctx.stroke();

    ctx.fillStyle = '#000';
    ctx.font = '12px Arial';
    ctx.textAlign = 'center';
    ctx.textBaseline = 'bottom';

    let displayLabel = label;
    if (label.length > 20) {
        displayLabel = label.substring(0, 17) + '...';
    }
    ctx.fillText(displayLabel, x, y - nodeRadius - 5);
}

function getTreeHeight(node) {
    if (!node) return 0;
    if (!node.children || node.children.length === 0) {
        return nodeRadius * 2 + verticalSpacing;
    }
    return nodeRadius * 2 + verticalSpacing + Math.max(...node.children.map(getTreeHeight));
}
